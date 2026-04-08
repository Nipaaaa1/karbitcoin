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

  blockchain.addBlock({t1});

  Transaction t2(addressA, addressB, 20);
  t2.publicKey = publicA;
  t2.signature = signData(privateA, t2.calculateHash());

  blockchain.addBlock({t2});

  std::cout << "Address " << addressA
            << " Balance: " << blockchain.getBalance(addressA) << "\n";
  std::cout << "Address " << addressB
            << " Balance: " << blockchain.getBalance(addressB) << "\n";
}
