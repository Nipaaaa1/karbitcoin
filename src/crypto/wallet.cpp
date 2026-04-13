#include "crypto/wallet.hpp"
#include "core/transaction.hpp"
#include "crypto/address.hpp"
#include "crypto/ecdsa.hpp"
#include <string>

Wallet::Wallet() {
  auto [privKey, pubKey] = generateKeyPair();
  publicKey = pubKey;
  privateKey = privKey;
  address = publicKeyToAddress(pubKey);
}

std::string Wallet::getPublicKey() { return publicKey; }

std::string Wallet::getAddress() { return address; }

void Wallet::signTransaction(Transaction &tx) {
  tx.publicKey = publicKey;
  tx.signature = signData(privateKey, tx.calculateHash());
}
