#include <gtest/gtest.h>
#include "crypto/ecdsa.hpp"

TEST(ECDSATest, KeyPairGeneration) {
    auto [priv, pub] = generateKeyPair();
    EXPECT_FALSE(priv.empty());
    EXPECT_FALSE(pub.empty());
    EXPECT_NE(priv, pub);
}

TEST(ECDSATest, SignAndVerifyValid) {
    auto [priv, pub] = generateKeyPair();
    std::string data = "transaction_data_123";
    
    std::string signature = signData(priv, data);
    EXPECT_FALSE(signature.empty());
    
    EXPECT_TRUE(verifySignature(pub, data, signature));
}

TEST(ECDSATest, VerifyInvalidData) {
    auto [priv, pub] = generateKeyPair();
    std::string data = "original_data";
    std::string tamperedData = "tampered_data";
    
    std::string signature = signData(priv, data);
    EXPECT_FALSE(verifySignature(pub, tamperedData, signature));
}

TEST(ECDSATest, VerifyInvalidSignature) {
    auto [priv, pub] = generateKeyPair();
    std::string data = "data";
    
    std::string signature = signData(priv, data);
    // Tamper with signature by changing last char if possible
    if(!signature.empty()) {
        signature[signature.size()-1] = (signature.back() == '0' ? '1' : '0');
    }
    
    EXPECT_FALSE(verifySignature(pub, data, signature));
}
