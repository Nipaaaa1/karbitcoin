#pragma once

#include "core/block.hpp"
#include "core/transaction.hpp"
#include <string>
#include <vector>

class Blockchain {
private:
  std::vector<Block> chain;
  std::vector<Transaction> mempool;
  UTXOset utxoSet;
  int difficulty;

public:
  Blockchain(int diff = 5);

  Transaction createCoinbase(const std::string &minerAddress, double fees);

  void addTransaction(const Transaction &tx);
  void applyTransaction(const Transaction &tx);
  void minePendingTransactions(const std::string &minerAddress);

  double getBalance(const std::string &address) const;
  size_t getHeight() const;
  const Block& getBlock(size_t index) const;
  void addBlock(const Block& block);
  UTXOset getUtxoSet();

  bool isValidBlock(const Block &current, const Block &previous) const;
  bool isChainValid() const;

private:
  bool isValidTransaction(const Transaction &tx) const;
  Block createGenesisBlock();
  const Block &getLatestBlock() const;
};
