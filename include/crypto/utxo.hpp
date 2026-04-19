#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>
#include <utility>
struct TxIn {
  std::string prevTxId;
  size_t outputIndex;
};

struct TxOut {
  std::string address;
  double amount;
};

using UTXOset = std::unordered_map<std::string, TxOut>;

std::string generateUtxoKey(const std::string &TxId, size_t index);

std::pair<std::string, size_t> parseKey(const std::string &key);

UTXOset getUtxoFromAddress(const std::string &address, UTXOset utxoSet);
