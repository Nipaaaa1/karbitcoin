#include "core/blockchain.hpp"
#include "core/transaction.hpp"
#include "crypto/wallet.hpp"
#include <iostream>
#include <string>

int main() {
  Blockchain blockchain(3);

  Wallet udin;
  Wallet jamal;

  Transaction t1("SYSTEM", udin.getAddress(), 50);

  blockchain.addTransaction({t1});
  blockchain.minePendingTransactions(udin.getAddress());

  Transaction t2(udin.getAddress(), jamal.getAddress(), 200);
  udin.signTransaction(t2);

  blockchain.addTransaction({t2});
  blockchain.minePendingTransactions(jamal.getAddress());

  std::cout << "Address " << udin.getAddress()
            << " Balance: " << blockchain.getBalance(udin.getAddress()) << "\n";
  std::cout << "Address " << jamal.getAddress()
            << " Balance: " << blockchain.getBalance(jamal.getAddress())
            << "\n";

  std::cout << "Chain valid: " << blockchain.isChainValid() << "\n";

  auto &chain = const_cast<std::vector<Block> &>(blockchain.getChain());
  chain[1].transactions[0].amount = 9999;

  std::cout << "After tamper: " << blockchain.isChainValid() << "\n";
}
