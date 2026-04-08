#include "core/transaction.hpp"
#include "crypto/hash.hpp"
#include <sstream>
#include <string>

Transaction::Transaction(const std::string &from, const std::string &to,
                         double amount)
    : from(from), to(to), amount(amount) {}

std::string Transaction::calculateHash() const {
  std::stringstream ss;
  ss << from << to << amount;
  return hashString(ss.str());
}
