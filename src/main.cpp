#include "core/blockchain.hpp"
#include "core/transaction.hpp"
#include "crypto/address.hpp"
#include "crypto/ecdsa.hpp"
#include <iostream>
#include <string>

int main() {
  Blockchain blockchain(3);

  auto [privateA, publicA] = generateKeyPair();
  auto [privateB, publicB] = generateKeyPair();

  std::string addressA = publicKeyToAddress(publicA);
  std::string addressB = publicKeyToAddress(publicB);

  Transaction t1("SYSTEM", addressA, 50);

  blockchain.addTransaction({t1});
  blockchain.minePendingTransactions(addressA);

  Transaction t2(addressA, addressB, 200);
  t2.publicKey = publicA;
  t2.signature = signData(privateA, t2.calculateHash());

  blockchain.addTransaction({t2});
  blockchain.minePendingTransactions(addressB);

  std::cout << "Address " << addressA
            << " Balance: " << blockchain.getBalance(addressA) << "\n";
  std::cout << "Address " << addressB
            << " Balance: " << blockchain.getBalance(addressB) << "\n";

  std::cout << "Chain valid: " << blockchain.isChainValid() << "\n";

  auto &chain = const_cast<std::vector<Block> &>(blockchain.getChain());
  chain[1].transactions[0].amount = 9999;

  std::cout << "After tamper: " << blockchain.isChainValid() << "\n";
}
