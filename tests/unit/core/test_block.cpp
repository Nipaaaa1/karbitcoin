#include <gtest/gtest.h>
#include "core/block.hpp"

TEST(BlockTest, HashCalculation) {
    std::vector<Transaction> txs;
    Block block(1, txs, "prev_hash", 2);
    
    std::string h1 = block.calculateHash();
    EXPECT_FALSE(h1.empty());
    
    block.nonce++;
    std::string h2 = block.calculateHash();
    EXPECT_NE(h1, h2);

    // Test difficulty impact on hash
    Block block2(1, txs, "prev_hash", 3);
    EXPECT_NE(block.calculateHash(), block2.calculateHash());
}

TEST(BlockTest, MiningTest) {
    std::vector<Transaction> txs;
    int difficulty = 2;
    Block block(1, txs, "prev_hash", difficulty);
    
    block.mine(difficulty);
    
    std::string prefix(difficulty, '0');
    EXPECT_EQ(block.hash.substr(0, difficulty), prefix);
    EXPECT_EQ(block.difficulty, difficulty);
}
