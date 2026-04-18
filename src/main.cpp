#include "core/blockchain.hpp"
#include "crypto/wallet.hpp"
#include <iostream>

int main() {
  Blockchain blockchain(3);

  Wallet udin;
  Wallet jamal;

  blockchain.minePendingTransactions(udin.getAddress());

  Transaction t2 = createTransaction(udin.getAddress(), jamal.getAddress(), 20, 1.0,
                                     blockchain.getUtxoSet());
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
  chain[1].transactions[0].outputs[0].amount = 9999;

  std::cout << "After tamper: " << blockchain.isChainValid() << "\n";
}
