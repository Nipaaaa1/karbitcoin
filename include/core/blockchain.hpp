#pragma once

#include "core/block.hpp"
#include "core/transaction.hpp"
#include <string>
#include <vector>

class Blockchain {
private:
  std::vector<Block> chain;
  std::vector<Transaction> mempool;
  int difficulty;

public:
  Blockchain(int diff = 5);

  void addTransaction(const Transaction &tx);
  void minePendingTransactions(const std::string &minerAddress);
  double getBalance(const std::string &address) const;
  const std::vector<Block> &getChain() const;

  bool isValidBlock(const Block &current, const Block &previous) const;
  bool isChainValid() const;

private:
  bool isValidTransaction(const Transaction &tx) const;
  Block createGenesisBlock();
  const Block &getLatestBlock() const;
};
