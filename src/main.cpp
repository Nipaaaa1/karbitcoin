#include "core/blockchain.hpp"
#include <iostream>

int main() {
  Blockchain blockchain(5);

  std::vector<Transaction> txs1 = {Transaction("SYSTEM", "jamal", 100)};

  blockchain.addBlock(txs1);

  std::vector<Transaction> txs2 = {Transaction("jamal", "udin", 30)};

  blockchain.addBlock(txs2);

  std::cout << "Balance Jamal: " << blockchain.getBalance("jamal") << "\n";
  std::cout << "Balance Udin: " << blockchain.getBalance("udin") << "\n";
}
