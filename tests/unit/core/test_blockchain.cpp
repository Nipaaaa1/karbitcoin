#include <gtest/gtest.h>
#include "core/blockchain.hpp"

TEST(BlockchainTest, Initialization) {
    Blockchain bc(1);
    EXPECT_EQ(bc.getHeight(), 1); // Genesis block
}

TEST(BlockchainTest, ChainValidity) {
    Blockchain bc(1);
    EXPECT_TRUE(bc.isChainValid());
}

TEST(BlockchainTest, MiningPendingTransactions) {
    Blockchain bc(1);
    std::string miner = "miner_addr";
    
    // Initial balance
    EXPECT_EQ(bc.getBalance(miner), 0.0);
    
    bc.minePendingTransactions(miner);
    
    EXPECT_EQ(bc.getHeight(), 2);
    // Coinbase reward is usually added, need to check blockchain.cpp for reward value
    EXPECT_GT(bc.getBalance(miner), 0.0);
}
