#include <gtest/gtest.h>
#include "crypto/utxo.hpp"

TEST(UTXOTest, KeyGenerationAndParsing) {
    std::string txId = "abc123";
    size_t index = 5;
    
    std::string key = generateUtxoKey(txId, index);
    auto [parsedId, parsedIndex] = parseKey(key);
    
    EXPECT_EQ(parsedId, txId);
    EXPECT_EQ(parsedIndex, index);
}

TEST(UTXOTest, GetUtxoFromAddress) {
    UTXOset fullSet;
    fullSet["tx1:0"] = {"addr1", 10.0};
    fullSet["tx1:1"] = {"addr2", 20.0};
    fullSet["tx2:0"] = {"addr1", 5.0};
    
    UTXOset addr1Set = getUtxoFromAddress("addr1", fullSet);
    
    EXPECT_EQ(addr1Set.size(), 2);
    EXPECT_EQ(addr1Set["tx1:0"].amount, 10.0);
    EXPECT_EQ(addr1Set["tx2:0"].amount, 5.0);
}
