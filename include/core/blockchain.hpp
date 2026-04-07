#pragma once

#include "core/block.hpp"
#include <vector>

class Blockchain {
private:
  std::vector<Block> chain;
  int difficulty;

public:
  Blockchain(int diff = 5);

  void addBlock(const std::string &data);
  const std::vector<Block> &getChain() const;

private:
  Block createGenesisBlock();
  const Block &getLatestBlock() const;
};
