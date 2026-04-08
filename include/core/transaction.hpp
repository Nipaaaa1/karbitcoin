#pragma once

#include <string>

class Transaction {
public:
  std::string from;
  std::string to;
  double amount;

  std::string signature;
  std::string publicKey;

  Transaction(const std::string &from, const std::string &to, double amount);

  std::string calculateHash() const;
};
