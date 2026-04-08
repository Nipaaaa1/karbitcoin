#include "core/transaction.hpp"
#include "crypto/ecdsa.hpp"
#include <iostream>

int main() {
  auto [keyPriv, keyPub] = generateKeyPair();

  Transaction tx("alice", "bob", 50);

  std::string hash = tx.calculateHash();

  tx.signature = signData(keyPriv, hash);
  tx.publicKey = keyPub;

  bool valid = verifySignature(tx.publicKey, hash, tx.signature);

  std::cout << "Signature valid: " << valid << "\n";
}
