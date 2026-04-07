#pragma once

#include "core/block.hpp"
#include <vector>

class Blockchain {
private:
  std::vector<Block> chain;

public:
  Blockchain();

  void addBlock(const std::string &data);
  const std::vector<Block> &getChain() const;

private:
  Block createGenesisBlock();
  const Block &getLatestBlock() const;
};
