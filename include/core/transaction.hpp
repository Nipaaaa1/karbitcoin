#pragma once

#include "crypto/utxo.hpp"
#include <string>
#include <vector>

class Transaction {
public:
  std::string id;
  std::vector<TxIn> inputs;
  std::vector<TxOut> outputs;

  std::string signature;
  std::string publicKey;

  Transaction(const std::vector<TxIn> &in, const std::vector<TxOut> &out);

  std::string calculateHash() const;
  void setId();
};

Transaction createTransaction(const std::string &from, const std::string &to,
                              double amount, UTXOset utxoSet);
