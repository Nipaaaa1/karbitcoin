#include <gtest/gtest.h>
#include "crypto/hash.hpp"

TEST(HashTest, ConsistencyTest) {
    std::string input = "karbitcoin";
    std::string hash1 = hashString(input);
    std::string hash2 = hashString(input);
    
    EXPECT_EQ(hash1, hash2);
}

TEST(HashTest, AvalancheEffect) {
    std::string input1 = "karbitcoin1";
    std::string input2 = "karbitcoin2";
    
    std::string hash1 = hashString(input1);
    std::string hash2 = hashString(input2);
    
    EXPECT_NE(hash1, hash2);
}

TEST(HashTest, EmptyString) {
    EXPECT_NO_THROW(hashString(""));
}
