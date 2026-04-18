#include "core/blockchain.hpp"
#include "core/block.hpp"
#include "crypto/ecdsa.hpp"
#include <stdexcept>
#include <string>
#include <vector>

Blockchain::Blockchain(int diff) : difficulty(diff) {
  Block genesisBlock = createGenesisBlock();
  genesisBlock.mine(difficulty);
  chain.push_back(genesisBlock);
}

Block Blockchain::createGenesisBlock() {
  return Block(0, std::vector<Transaction>{}, "0");
}

const Block &Blockchain::getLatestBlock() const { return chain.back(); }

void Blockchain::addTransaction(const Transaction &tx) {
  if (!isValidTransaction(tx)) {
    return;
  }
  mempool.push_back(tx);
}

void Blockchain::applyTransaction(const Transaction &tx) {
  for (const auto &input : tx.inputs) {
    std::string key = generateUtxoKey(input.prevTxId, input.outputIndex);
    if (utxoSet.find(key) == utxoSet.end()) {
      throw std::runtime_error("Invalid Transaciton");
    }
    utxoSet.erase(key);
  }

  for (size_t i = 0; i < tx.outputs.size(); i++) {
    std::string key = generateUtxoKey(tx.id, i);

    utxoSet[key] = tx.outputs[i];
  }
}

Transaction Blockchain::createCoinbase(const std::string &minerAddress) {
  Transaction tx({}, {{minerAddress, 50}});
  return tx;
}

void Blockchain::minePendingTransactions(const std::string &minerAddress) {
  Transaction coinbase = createCoinbase(minerAddress);
  mempool.insert(mempool.begin(), coinbase);

  const Block &prev = getLatestBlock();
  Block newBlock(chain.size(), mempool, prev.hash);

  newBlock.mine(difficulty);
  chain.push_back(newBlock);

  for (const auto &tx : mempool) {
    applyTransaction(tx);
  }

  mempool.clear();
}

double Blockchain::getBalance(const std::string &address) const {
  double balance = 0;

  for (const auto &[key, value] : getUtxoFromAddress(address, utxoSet)) {
    balance += value.amount;
  }
  return balance;
}

UTXOset Blockchain::getUtxoSet() { return utxoSet; }

bool Blockchain::isValidTransaction(const Transaction &tx) const {
  if (tx.inputs.empty()) {
    return true;
  }

  if (!verifySignature(tx.publicKey, tx.calculateHash(), tx.signature)) {
    return false;
  }

  for (const auto &input : tx.inputs) {
    std::string key = generateUtxoKey(input.prevTxId, input.outputIndex);
    if (utxoSet.find(key) == utxoSet.end()) {
      return false;
    }
  }

  return true;
}

const std::vector<Block> &Blockchain::getChain() const { return chain; }

bool Blockchain::isValidBlock(const Block &current,
                              const Block &previous) const {
  if (current.previousHash != previous.hash) {
    return false;
  }

  if (current.hash != current.calculateHash()) {
    return false;
  }

  std::string target(difficulty, '0');

  if (current.hash.substr(0, difficulty) != target) {
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
