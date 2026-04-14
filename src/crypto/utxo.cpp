#include "crypto/utxo.hpp"
#include <cstddef>
#include <string>
#include <utility>

std::string generateUtxoKey(const std::string &TxId, size_t index) {
  return TxId + ":" + std::to_string(index);
}

std::pair<std::string, size_t> parseKey(const std::string &key) {
  auto pos = key.find(":");
  return std::make_pair(key.substr(0, pos), std::stoul(key.substr(pos + 1)));
}

UTXOset getUtxoFromAddress(const std::string &address, UTXOset utxoSet) {

  UTXOset result;
  for (const auto &[key, value] : utxoSet) {
    if (value.address == address) {
      result[key] = value;
    }
  }
  return result;
}
