#pragma once

#include "core/block.hpp"
#include "core/transaction.hpp"
#include <string>
#include <vector>

class Blockchain {
private:
  std::vector<Block> chain;
  int difficulty;

public:
  Blockchain(int diff = 5);

  void addBlock(const std::vector<Transaction> &txs);
  double getBalance(const std::string &address) const;
  bool isValidTransartion(const Transaction &tx) const;
  const std::vector<Block> &getChain() const;

private:
  Block createGenesisBlock();
  const Block &getLatestBlock() const;
};
