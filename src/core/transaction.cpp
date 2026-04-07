#include "core/transaction.hpp"
#include <string>

Transaction::Transaction(const std::string &from, const std::string &to,
                         double amount)
    : from(from), to(to), amount(amount) {}
