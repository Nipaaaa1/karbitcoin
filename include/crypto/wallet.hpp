#include "core/transaction.hpp"
#include <string>

class Wallet {
private:
  std::string publicKey;
  std::string privateKey;
  std::string address;

public:
  Wallet();

  std::string getPublicKey();
  std::string getAddress();
  void signTransaction(Transaction &tx);
};
