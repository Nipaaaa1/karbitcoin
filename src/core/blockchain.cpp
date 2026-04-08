#include "core/blockchain.hpp"
#include "core/block.hpp"
#include "core/transaction.hpp"
#include "crypto/address.hpp"
#include "crypto/ecdsa.hpp"
#include <iostream>
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

  if (tx.from != "SYSTEM") {
    double balance = getBalance(tx.from) - tx.amount;

    for (const auto &t : mempool) {
      if (t.from == tx.from) {
        balance -= t.amount;
      }
    }

    if (balance < 0) {
      return;
    }
  }

  mempool.push_back(tx);
}

void Blockchain::minePendingTransactions(const std::string &minerAddress) {
  Transaction reward("SYSTEM", minerAddress, 50);
  mempool.push_back(reward);

  const Block &prev = getLatestBlock();
  Block newBlock(chain.size(), mempool, prev.hash);

  newBlock.mine(difficulty);
  chain.push_back(newBlock);

  mempool.clear();
}

double Blockchain::getBalance(const std::string &address) const {
  double balance = 0;

  for (const auto &block : chain) {
    for (const auto &tx : block.transactions) {
      if (tx.from == address) {
        balance -= tx.amount;
      }

      if (tx.to == address) {
        balance += tx.amount;
      }
    }
  }

  return balance;
}

bool Blockchain::isValidTransaction(const Transaction &tx) const {
  if (tx.from == "SYSTEM")
    return true;

  if (!verifySignature(tx.publicKey, tx.calculateHash(), tx.signature)) {
    return false;
  }

  std::string derived = publicKeyToAddress(tx.publicKey);
  if (derived != tx.from) {
    return false;
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
