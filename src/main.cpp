#include "core/blockchain.hpp"
#include <iostream>

int main() {
  Blockchain blockchain;

  blockchain.addBlock("Transaction 1");
  blockchain.addBlock("Transaction 2");

  for (const auto &block : blockchain.getChain()) {
    std::cout << "Index: " << block.index << "\n";
    std::cout << "Data: " << block.data << "\n";
    std::cout << "Hash: " << block.hash << "\n";
    std::cout << "Prev: " << block.previousHash << "\n\n";
  }

  return 0;
}
