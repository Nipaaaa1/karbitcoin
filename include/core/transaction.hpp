#pragma once

#include "crypto/utxo.hpp"
#include <string>
#include <vector>

class Transaction {
public:
  std::string id;
  long timestamp;
  std::vector<TxIn> inputs;
  std::vector<TxOut> outputs;

  std::string signature;
  std::string publicKey;

  Transaction() = default;
  Transaction(const std::vector<TxIn> &in, const std::vector<TxOut> &out);

  std::string calculateHash() const;
  void setId();

  double getValueIn(const UTXOset &utxoSet) const;
  double getValueOut() const;
};

Transaction createTransaction(const std::string &from, const std::string &to,
                              double amount, double fee, UTXOset utxoSet);
