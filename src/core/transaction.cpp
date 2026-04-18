#include "core/transaction.hpp"
#include "crypto/hash.hpp"
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

Transaction::Transaction(const std::vector<TxIn> &in,
                         const std::vector<TxOut> &out)
    : inputs(in), outputs(out) {
  setId();
}

void Transaction::setId() { id = calculateHash(); }

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
    throw std::runtime_error("No utxo from this address");
  }

  std::vector<std::pair<std::string, TxOut>> utxoList;
  for (const auto &item : addressUtxo) {
    utxoList.push_back(item);
  }

  std::sort(utxoList.begin(), utxoList.end(), [](const auto &a, const auto &b) {
    return a.second.amount > b.second.amount;
  });

  std::vector<TxIn> inputs;
  double totalFound = 0;

  for (const auto &[key, utxo] : utxoList) {
    auto [txId, index] = parseKey(key);
    inputs.push_back({txId, index});
    totalFound += utxo.amount;

    if (totalFound >= amount) {
      break;
    }
  }

  if (totalFound < amount) {
    throw std::runtime_error("Insufficient funds: have " +
                             std::to_string(totalFound) + ", need " +
                             std::to_string(amount));
  }

  std::vector<TxOut> outputs;
  outputs.push_back({to, amount});

  if (totalFound - amount > 0) {
    outputs.push_back({from, totalFound - amount});
  }

  return Transaction(inputs, outputs);
}
