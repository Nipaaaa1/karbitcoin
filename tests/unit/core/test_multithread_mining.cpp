#include <gtest/gtest.h>
#include "core/block.hpp"
#include <chrono>

TEST(MultiThreadMiningTest, BasicMultiThreadMining) {
    std::vector<Transaction> txs;
    int difficulty = 3;
    Block block(1, txs, "prev_hash", difficulty);
    
    // Test with 4 threads
    block.mine(difficulty, 4);
    
    std::string prefix(difficulty, '0');
    EXPECT_EQ(block.hash.substr(0, difficulty), prefix);
}

TEST(MultiThreadMiningTest, ConsistencyTest) {
    std::vector<Transaction> txs;
    int difficulty = 2;
    
    Block block1(1, txs, "prev_hash", difficulty);
    block1.mine(difficulty, 1); // 1 thread
    
    Block block2(1, txs, "prev_hash", difficulty);
    block2.mine(difficulty, 4); // 4 threads
    
    // Both should find a valid hash (though nonces might differ if multiple solutions exist, 
    // but usually they find the same first one if strategy is consistent).
    // Actually, with different thread counts and stride, they might find different valid nonces.
    // The important thing is that BOTH are valid.
    
    std::string prefix(difficulty, '0');
    EXPECT_EQ(block1.hash.substr(0, difficulty), prefix);
    EXPECT_EQ(block2.hash.substr(0, difficulty), prefix);
    
    EXPECT_EQ(block1.hash, block1.calculateHash());
    EXPECT_EQ(block2.hash, block2.calculateHash());
}
