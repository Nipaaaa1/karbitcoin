#include "crypto/ecdsa.hpp"

#include <cstddef>
#include <cstdlib>
#include <iomanip>
#include <ios>
#include <openssl/bio.h>
#include <openssl/crypto.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

static std::string toHex(const unsigned char *data, size_t len) {
  std::stringstream ss;
  for (size_t i = 0; i < len; i++) {
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)data[i];
  }
  return ss.str();
}

static std::vector<unsigned char> fromHex(const std::string &hex) {
  std::vector<unsigned char> out;
  for (size_t i = 0; i < hex.length(); i += 2) {
    std::string byte = hex.substr(i, 2);
    out.push_back(
        static_cast<unsigned char>(strtol(byte.c_str(), nullptr, 16)));
  }
  return out;
}

std::pair<std::string, std::string> generateKeyPair() {
  EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_from_name(nullptr, "EC", nullptr);
  EVP_PKEY *pkey = nullptr;

  EVP_PKEY_keygen_init(ctx);

  EVP_PKEY_CTX_set_group_name(ctx, "secp256k1");

  EVP_PKEY_keygen(ctx, &pkey);

  BIO *pri = BIO_new(BIO_s_mem());
  BIO *pub = BIO_new(BIO_s_mem());

  PEM_write_bio_PrivateKey(pri, pkey, nullptr, nullptr, 0, nullptr, nullptr);

  PEM_write_bio_PUBKEY(pub, pkey);

  char *priData;
  long priLen = BIO_get_mem_data(pri, &priData);

  char *pubData;
  long pubLen = BIO_get_mem_data(pub, &pubData);

  std::string privateKey(priData, priLen);
  std::string publicKey(pubData, pubLen);

  BIO_free(pri);
  BIO_free(pub);
  EVP_PKEY_free(pkey);
  EVP_PKEY_CTX_free(ctx);

  return {privateKey, publicKey};
}

std::string signData(const std::string &privateKeyPem,
                     const std::string &data) {
  BIO *bio = BIO_new_mem_buf(privateKeyPem.data(), privateKeyPem.size());
  EVP_PKEY *pkey = PEM_read_bio_PrivateKey(bio, nullptr, nullptr, nullptr);

  EVP_MD_CTX *ctx = EVP_MD_CTX_new();

  EVP_DigestSignInit(ctx, nullptr, EVP_sha256(), nullptr, pkey);

  EVP_DigestSignUpdate(ctx, data.data(), data.size());

  size_t sigLen = 0;
  EVP_DigestSignFinal(ctx, nullptr, &sigLen);

  std::vector<unsigned char> sig(sigLen);

  EVP_DigestSignFinal(ctx, sig.data(), &sigLen);

  BIO_free(bio);
  EVP_MD_CTX_free(ctx);
  EVP_PKEY_free(pkey);

  return toHex(sig.data(), sigLen);
}

bool verifySignature(const std::string &publicKeyPem, const std::string &data,
                     const std::string &signatureHex) {
  BIO *bio = BIO_new_mem_buf(publicKeyPem.data(), publicKeyPem.size());
  EVP_PKEY *pkey = PEM_read_bio_PUBKEY(bio, nullptr, nullptr, nullptr);

  EVP_MD_CTX *ctx = EVP_MD_CTX_new();

  EVP_DigestVerifyInit(ctx, nullptr, EVP_sha256(), nullptr, pkey);

  EVP_DigestVerifyUpdate(ctx, data.data(), data.size());

  auto sig = fromHex(signatureHex);

  int result = EVP_DigestVerifyFinal(ctx, sig.data(), sig.size());

  BIO_free(bio);
  EVP_MD_CTX_free(ctx);
  EVP_PKEY_free(pkey);

  return result == 1;
}
