#pragma once

#include "core/transaction.hpp"
#include <string>
#include <vector>
class Block {
public:
  int index;
  long timestamp;
  std::vector<Transaction> transactions;
  std::string previousHash;
  std::string hash;
  int nonce;

  Block() = default;
  Block(const int &idx, const std::vector<Transaction> &txs,
        const std::string &prevHash);

  std::string calculateHash() const;
  void mine(int difficulty);
};
