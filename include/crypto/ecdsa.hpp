#pragma once
#include <string>
#include <utility>

std::pair<std::string, std::string> generateKeyPair();

std::string signData(const std::string &privateKeyPem, const std::string &data);

bool verifySignature(const std::string &privateKeyPem, const std::string &data,
                     const std::string &signatureHex);
