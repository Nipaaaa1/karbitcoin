#include "core/blockchain.hpp"

Blockchain::Blockchain() { chain.push_back(createGenesisBlock()); }

Block Blockchain::createGenesisBlock() {
  return Block(0, "Genesis Block", "0");
}

const Block &Blockchain::getLatestBlock() const { return chain.back(); }

void Blockchain::addBlock(const std::string &data) {
  const Block &prev = getLatestBlock();
  Block newBlock(chain.size(), data, prev.hash);
  chain.push_back(newBlock);
}

const std::vector<Block> &Blockchain::getChain() const { return chain; }
