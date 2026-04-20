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
  std::string getPrivateKey();
  std::string getAddress();
  void signTransaction(Transaction &tx);

  void saveToFile(const std::string& filename);
  void loadFromFile(const std::string& filename);
  
  void setKeys(const std::string& priv, const std::string& pub);
};
