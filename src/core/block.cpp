#include "core/block.hpp"
#include "crypto/hash.hpp"
#include <ctime>
#include <sstream>
#include <string>

Block::Block(const int &idx, const std::string &data,
             const std::string &prevHash)
    : index(idx), data(data), previousHash(prevHash), nonce(0) {
  timestamp = std::time(nullptr);
  hash = calculateHash();
}

std::string Block::calculateHash() const {
  std::stringstream ss;
  ss << index << timestamp << data << previousHash << nonce;
  return hashString(ss.str());
}

void Block::mine(int difficulty) {
  std::string target(difficulty, '0');

  while (hash.substr(0, difficulty) != target) {
    nonce++;
    hash = calculateHash();
  }
}
