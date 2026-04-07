#pragma once

#include <string>
class Block {
public:
  int index;
  long timestamp;
  std::string data;
  std::string previousHash;
  std::string hash;

  Block(const int &idx, const std::string &data, const std::string &prevHash);

  std::string calculateHash() const;
};
