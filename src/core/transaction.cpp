#include "core/transaction.hpp"
#include "crypto/hash.hpp"
#include <algorithm>
#include <ctime>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

Transaction::Transaction(const std::vector<TxIn> &in,
                         const std::vector<TxOut> &out)
    : inputs(in), outputs(out) {
  timestamp = std::time(nullptr);
  setId();
}

void Transaction::setId() { id = calculateHash(); }

std::string Transaction::calculateHash() const {
  std::stringstream ss;
  ss << timestamp;
  for (const auto &in : inputs) {
    ss << in.prevTxId << in.outputIndex;
  }

  for (const auto &out : outputs) {
    ss << out.address << out.amount;
  }
  return hashString(ss.str());
}

double Transaction::getValueIn(const UTXOset &utxoSet) const {
  double total = 0;
  for (const auto &input : inputs) {
    std::string key = generateUtxoKey(input.prevTxId, input.outputIndex);
    auto it = utxoSet.find(key);
    if (it != utxoSet.end()) {
      total += it->second.amount;
    }
  }
  return total;
}

double Transaction::getValueOut() const {
  double total = 0;
  for (const auto &out : outputs) {
    total += out.amount;
  }
  return total;
}

Transaction createTransaction(const std::string &from, const std::string &to,
                              double amount, double fee, UTXOset utxoSet) {
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
  double needed = amount + fee;

  for (const auto &[key, utxo] : utxoList) {
    auto [txId, index] = parseKey(key);
    inputs.push_back({txId, index});
    totalFound += utxo.amount;

    if (totalFound >= needed) {
      break;
    }
  }

  if (totalFound < needed) {
    throw std::runtime_error("Insufficient funds (including fee): have " +
                             std::to_string(totalFound) + ", need " +
                             std::to_string(needed));
  }

  std::vector<TxOut> outputs;
  outputs.push_back({to, amount});

  if (totalFound - needed > 0) {
    outputs.push_back({from, totalFound - needed});
  }

  return Transaction(inputs, outputs);
}
