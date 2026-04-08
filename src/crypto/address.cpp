#include "crypto/address.hpp"
#include "crypto/hash.hpp"
#include <string>

std::string publicKeyToAddress(const std::string &publicKeyPem) {
  return hashString(publicKeyPem).substr(0, 40);
}
