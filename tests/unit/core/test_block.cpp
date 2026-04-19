#include <gtest/gtest.h>
#include "core/block.hpp"

TEST(BlockTest, HashCalculation) {
    std::vector<Transaction> txs;
    Block block(1, txs, "prev_hash");
    
    std::string h1 = block.calculateHash();
    EXPECT_FALSE(h1.empty());
    
    block.nonce++;
    std::string h2 = block.calculateHash();
    EXPECT_NE(h1, h2);
}

TEST(BlockTest, MiningTest) {
    std::vector<Transaction> txs;
    Block block(1, txs, "prev_hash");
    
    int difficulty = 2; // Low difficulty for fast test
    block.mine(difficulty);
    
    std::string prefix(difficulty, '0');
    EXPECT_EQ(block.hash.substr(0, difficulty), prefix);
}
