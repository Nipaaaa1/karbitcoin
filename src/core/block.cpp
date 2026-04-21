#include "core/block.hpp"
#include "crypto/hash.hpp"
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

Block::Block(const int &idx, const std::vector<Transaction> &txs,
             const std::string &prevHash, int diff)
    : index(idx), transactions(txs), previousHash(prevHash), nonce(0), difficulty(diff) {
  timestamp = std::time(nullptr);
  hash = calculateHash();
}

static std::string transactionToString(const std::vector<Transaction> &txs) {
  std::stringstream ss;
  for (const auto &tx : txs) {
    ss << tx.id << tx.signature;
    for (const auto &in : tx.inputs) {
      ss << in.prevTxId << in.outputIndex;
    }
    for (const auto &out : tx.outputs) {
      ss << out.address << out.amount;
    }
  }
  return ss.str();
}

std::string Block::calculateHash() const {
  std::stringstream ss;
  ss << index << timestamp << transactionToString(transactions) << previousHash
     << nonce << difficulty;
  return hashString(ss.str());
}

void Block::mine(int difficulty) {
  std::string target(difficulty, '0');

  std::cout << "Mining block...\n";
  while (hash.substr(0, difficulty) != target) {
    nonce++;
    hash = calculateHash();
  }
}
