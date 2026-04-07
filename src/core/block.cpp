#include "core/block.hpp"
#include "crypto/hash.hpp"
#include <ctime>

Block::Block(const int &idx, const std::string &data,
             const std::string &prevHash)
    : index(idx), data(data), previousHash(prevHash) {
  timestamp = std::time(nullptr);
  hash = calculateHash();
}

std::string Block::calculateHash() const {
  std::string input =
      std::to_string(index) + std::to_string(timestamp) + data + previousHash;

  return hashString(input);
}
