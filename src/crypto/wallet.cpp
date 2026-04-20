#include "crypto/wallet.hpp"
#include "core/transaction.hpp"
#include "crypto/address.hpp"
#include "crypto/ecdsa.hpp"
#include "core/storage.hpp"
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

Wallet::Wallet() {
  auto [privKey, pubKey] = generateKeyPair();
  publicKey = pubKey;
  privateKey = privKey;
  address = publicKeyToAddress(pubKey);
}

std::string Wallet::getPublicKey() { return publicKey; }

std::string Wallet::getPrivateKey() { return privateKey; }

std::string Wallet::getAddress() { return address; }

void Wallet::signTransaction(Transaction &tx) {
  tx.publicKey = publicKey;
  tx.signature = signData(privateKey, tx.calculateHash());
}

void Wallet::saveToFile(const std::string& filename) {
    nlohmann::json j = {
        {"privateKey", privateKey},
        {"publicKey", publicKey}
    };
    Storage::saveJson(filename, j);
}

void Wallet::loadFromFile(const std::string& filename) {
    nlohmann::json j = Storage::loadJson(filename);
    if (!j.is_null()) {
        privateKey = j.at("privateKey").get<std::string>();
        publicKey = j.at("publicKey").get<std::string>();
        address = publicKeyToAddress(publicKey);
    }
}

void Wallet::setKeys(const std::string& priv, const std::string& pub) {
    privateKey = priv;
    publicKey = pub;
    address = publicKeyToAddress(publicKey);
}
