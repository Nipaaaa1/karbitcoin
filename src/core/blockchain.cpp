#include "core/blockchain.hpp"
#include "core/block.hpp"
#include "core/storage.hpp"
#include "network/serialization.hpp"
#include "crypto/ecdsa.hpp"
#include <algorithm>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs = std::filesystem;

Blockchain::Blockchain(int diff, const std::string& dir) : difficulty(diff), dataDir(dir) {
  if (fs::exists(dataDir + "/blocks")) {
    loadFromDisk();
  }
  
  if (fs::exists(dataDir + "/utxo.json")) {
    loadUTXOSet();
  }

  if (fs::exists(dataDir + "/metadata.json")) {
    loadMetadata();
  }
  
  if (chain.empty()) {
    Block genesisBlock = createGenesisBlock();
    genesisBlock.mine(difficulty);
    chain.push_back(genesisBlock);
    saveBlock(genesisBlock);
    saveUTXOSet();
    saveMetadata();
  }
}

void Blockchain::saveMetadata() const {
    std::lock_guard<std::recursive_mutex> lock(blockchain_mutex_);
    nlohmann::json j = {
        {"difficulty", difficulty},
        {"height", chain.size()}
    };
    Storage::saveJson(dataDir + "/metadata.json", j);
}

void Blockchain::loadMetadata() {
    std::lock_guard<std::recursive_mutex> lock(blockchain_mutex_);
    nlohmann::json j = Storage::loadJson(dataDir + "/metadata.json");
    if (!j.is_null()) {
        difficulty = j.at("difficulty").get<int>();
    }
}

void Blockchain::saveUTXOSet() const {
    std::lock_guard<std::recursive_mutex> lock(blockchain_mutex_);
    nlohmann::json j = utxoSet;
    Storage::saveJson(dataDir + "/utxo.json", j);
}

void Blockchain::loadUTXOSet() {
    std::lock_guard<std::recursive_mutex> lock(blockchain_mutex_);
    nlohmann::json j = Storage::loadJson(dataDir + "/utxo.json");
    if (!j.is_null()) {
        utxoSet = j.get<UTXOset>();
    }
}

void Blockchain::saveBlock(const Block& block) const {
    std::string path = dataDir + "/blocks/block_" + std::to_string(block.index) + ".json";
    nlohmann::json j = block;
    Storage::saveJson(path, j);
}

void Blockchain::loadFromDisk() {
    std::lock_guard<std::recursive_mutex> lock(blockchain_mutex_);
    std::string blocksDir = dataDir + "/blocks";
    if (!fs::exists(blocksDir)) return;

    std::vector<std::string> blockFiles;
    for (const auto& entry : fs::directory_iterator(blocksDir)) {
        if (entry.path().extension() == ".json") {
            blockFiles.push_back(entry.path().string());
        }
    }

    // Sort by index to ensure correct order
    std::sort(blockFiles.begin(), blockFiles.end(), [](const std::string& a, const std::string& b) {
        // Simple extraction of index from filename "block_N.json"
        auto getIndex = [](const std::string& path) {
            std::string filename = fs::path(path).stem().string();
            return std::stoi(filename.substr(6));
        };
        return getIndex(a) < getIndex(b);
    });

    for (const auto& file : blockFiles) {
        nlohmann::json j = Storage::loadJson(file);
        if (!j.is_null()) {
            Block b = j.get<Block>();
            if (chain.empty()) {
                chain.push_back(b);
                for (const auto& tx : b.transactions) {
                    applyTransaction(tx);
                }
            } else if (isValidBlock(b, chain.back())) {
                chain.push_back(b);
                for (const auto& tx : b.transactions) {
                    applyTransaction(tx);
                }
            }
        }
    }
}

Block Blockchain::createGenesisBlock() {
  // Use a fixed timestamp for deterministic genesis hash across all nodes
  return Block(0, std::vector<Transaction>{}, "0", difficulty, 1713830400); // 2024-04-23
}

const Block &Blockchain::getLatestBlock() const { return chain.back(); }

void Blockchain::addTransaction(const Transaction &tx) {
  std::lock_guard<std::recursive_mutex> lock(blockchain_mutex_);
  if (!isValidTransaction(tx)) {
    return;
  }

  // Double-spending prevention within mempool
  for (const auto &pendingTx : mempool) {
    for (const auto &pendingIn : pendingTx.inputs) {
      for (const auto &newIn : tx.inputs) {
        if (pendingIn.prevTxId == newIn.prevTxId &&
            pendingIn.outputIndex == newIn.outputIndex) {
          return; // Already spent in mempool
        }
      }
    }
  }

  mempool.push_back(tx);
}

void Blockchain::applyTransaction(const Transaction &tx) {
  std::lock_guard<std::recursive_mutex> lock(blockchain_mutex_);
  // Coinbase transactions only have outputs
  if (!tx.inputs.empty()) {
    for (const auto &input : tx.inputs) {
      std::string key = generateUtxoKey(input.prevTxId, input.outputIndex);
      if (utxoSet.find(key) == utxoSet.end()) {
        throw std::runtime_error("Invalid Transaction: UTXO not found");
      }
      utxoSet.erase(key);
    }
  }

  for (size_t i = 0; i < tx.outputs.size(); i++) {
    std::string key = generateUtxoKey(tx.id, i);
    utxoSet[key] = tx.outputs[i];
  }
}

Transaction Blockchain::createCoinbase(const std::string &minerAddress,
                                       double fees) {
  double blockReward = 50.0;
  Transaction tx({}, {{minerAddress, blockReward + fees}});
  return tx;
}

int Blockchain::getAdjustedDifficulty() {
  if (chain.size() == 0 || chain.size() % DIFFICULTY_ADJUSTMENT_INTERVAL != 0) {
    return difficulty;
  }

  const Block &latestBlock = chain.back();
  // Get the block from the start of the current interval
  const Block &prevAdjustmentBlock = chain[chain.size() - DIFFICULTY_ADJUSTMENT_INTERVAL];

  long timeExpected = DIFFICULTY_ADJUSTMENT_INTERVAL * EXPECTED_BLOCK_TIME;
  long timeTaken = latestBlock.timestamp - prevAdjustmentBlock.timestamp;

  if (timeTaken < timeExpected) {
    return difficulty + 1;
  } else if (timeTaken > timeExpected) {
    return (difficulty > 1) ? difficulty - 1 : 1;
  }

  return difficulty;
}

void Blockchain::minePendingTransactions(const std::string &minerAddress) {
  std::lock_guard<std::recursive_mutex> lock(blockchain_mutex_);
  difficulty = getAdjustedDifficulty();
  
  double totalFees = 0;
  for (const auto &tx : mempool) {
    totalFees += tx.getValueIn(utxoSet) - tx.getValueOut();
  }

  Transaction coinbase = createCoinbase(minerAddress, totalFees);
  mempool.insert(mempool.begin(), coinbase);

  const Block &prev = getLatestBlock();
  Block newBlock(chain.size(), mempool, prev.hash, difficulty);

  newBlock.mine(difficulty);
  chain.push_back(newBlock);
  saveBlock(newBlock);

  for (const auto &tx : mempool) {
    applyTransaction(tx);
  }

  saveUTXOSet();
  saveMetadata();
  mempool.clear();
}

double Blockchain::getBalance(const std::string &address) const {
  std::lock_guard<std::recursive_mutex> lock(blockchain_mutex_);
  double balance = 0;

  for (const auto &[key, value] : getUtxoFromAddress(address, utxoSet)) {
    balance += value.amount;
  }
  return balance;
}

UTXOset Blockchain::getUtxoSet() { 
  std::lock_guard<std::recursive_mutex> lock(blockchain_mutex_);
  return utxoSet; 
}

bool Blockchain::isValidTransaction(const Transaction &tx) const {
  // Case 1: Coinbase Transaction (generated by the system)
  if (tx.inputs.empty()) {
    // Only 1 output allowed in coinbase for reward + fees
    if (tx.outputs.size() != 1)
      return false;
    return true;
  }

  // Case 2: Regular Transaction
  if (!verifySignature(tx.publicKey, tx.calculateHash(), tx.signature)) {
    return false;
  }

  double valueIn = 0;
  for (const auto &input : tx.inputs) {
    std::string key = generateUtxoKey(input.prevTxId, input.outputIndex);
    auto it = utxoSet.find(key);
    if (it == utxoSet.end()) {
      return false; // UTXO does not exist or already spent
    }
    valueIn += it->second.amount;
  }

  double valueOut = tx.getValueOut();
  if (valueIn < valueOut) {
    return false; // Inflation attempt: spending more than owned
  }

  return true;
}

size_t Blockchain::getHeight() const { 
  std::lock_guard<std::recursive_mutex> lock(blockchain_mutex_);
  return chain.size(); 
}

const Block& Blockchain::getBlock(size_t index) const {
    std::lock_guard<std::recursive_mutex> lock(blockchain_mutex_);
    if (index >= chain.size()) {
        throw std::runtime_error("Block index out of bounds");
    }
    return chain[index];
}

void Blockchain::addBlock(const Block& block) {
    std::lock_guard<std::recursive_mutex> lock(blockchain_mutex_);
    int expectedDifficulty = getAdjustedDifficulty();
    if (block.difficulty != expectedDifficulty) {
        throw std::runtime_error("Invalid block difficulty: expected " + 
                                 std::to_string(expectedDifficulty) + " but got " + 
                                 std::to_string(block.difficulty));
    }

    if (isValidBlock(block, getLatestBlock())) {
        chain.push_back(block);
        difficulty = expectedDifficulty;
        saveBlock(block);
        for (const auto& tx : block.transactions) {
            applyTransaction(tx);
            // Remove from mempool
            auto it = std::find_if(mempool.begin(), mempool.end(), [&](const Transaction& t) {
                return t.id == tx.id;
            });
            if (it != mempool.end()) {
                mempool.erase(it);
            }
        }
        saveUTXOSet();
        saveMetadata();
    } else {
        if (block.previousHash != getLatestBlock().hash) {
             throw std::runtime_error("Invalid block: previous hash mismatch. Expected " + getLatestBlock().hash + " but got " + block.previousHash);
        }
        if (block.hash != block.calculateHash()) {
             throw std::runtime_error("Invalid block: hash mismatch");
        }
        throw std::runtime_error("Invalid block offered to blockchain (general validation failed)");
    }
}

bool Blockchain::isValidBlock(const Block &current,
                              const Block &previous) const {
  if (current.previousHash != previous.hash) {
    return false;
  }

  if (current.hash != current.calculateHash()) {
    return false;
  }

  std::string target(current.difficulty, '0');

  if (current.hash.substr(0, current.difficulty) != target) {
    return false;
  }

  return true;
}

bool Blockchain::isChainValid() const {
  for (size_t i = 1; i < chain.size(); i++) {
    if (!isValidBlock(chain[i], chain[i - 1])) {
      return false;
    }
  }

  return true;
}
