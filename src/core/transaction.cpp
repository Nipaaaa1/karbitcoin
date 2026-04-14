#include "core/transaction.hpp"
#include "crypto/hash.hpp"
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

Transaction::Transaction(const std::vector<TxIn> &in,
                         const std::vector<TxOut> &out)
    : inputs(in), outputs(out) {}

std::string Transaction::calculateHash() const {
  std::stringstream ss;
  for (const auto &in : inputs) {
    ss << in.prevTxId << in.outputIndex;
  }

  for (const auto &out : outputs) {
    ss << out.address << out.amount;
  }
  return hashString(ss.str());
}

Transaction createTransaction(const std::string &from, const std::string &to,
                              double amount, UTXOset utxoSet) {
  UTXOset addressUtxo = getUtxoFromAddress(from, utxoSet);

  if (addressUtxo.empty()) {
    throw std::runtime_error("No utxo from this address\n");
  }

  std::vector<TxOut> utxoList;

  for (const auto &[key, value] : addressUtxo) {
    utxoList.push_back(value);
  }

  std::sort(utxoList.begin(), utxoList.end(),
            [](const TxOut &a, const TxOut &b) { return a.amount > b.amount; });

  std::vector<TxOut> selected;
  double total = 0;

  for (const auto &utxo : utxoList) {
    if (utxo.amount == amount) {
      selected.clear();
      selected.push_back(utxo);
    }

    total += utxo.amount;

    if (total > amount) {
      break;
    }
  }

  if (total < amount) {
    throw std::runtime_error("Insufficient funds");
  }

  std::vector<TxIn> inputs;

  for (const auto &utxo : selected) {
    for (const auto &[key, value] : addressUtxo) {
      if (value.amount == utxo.amount) {
        auto [txId, index] = parseKey(key);
        inputs.push_back({txId, index});
      }
    }
  }

  std::vector<TxOut> outputs;

  outputs.push_back({to, amount});

  if (total - amount > 0) {
    outputs.push_back({from, total - amount});
  }

  return {inputs, outputs};
}
