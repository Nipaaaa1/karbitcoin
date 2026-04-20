#include <gtest/gtest.h>
#include <filesystem>
#include "core/blockchain.hpp"

namespace fs = std::filesystem;

class BlockchainTest : public ::testing::Test {
protected:
    std::string test_data_dir = "test_bc_data";

    void SetUp() override {
        if (fs::exists(test_data_dir)) {
            fs::remove_all(test_data_dir);
        }
    }

    void TearDown() override {
        if (fs::exists(test_data_dir)) {
            fs::remove_all(test_data_dir);
        }
    }
};

TEST_F(BlockchainTest, Initialization) {
    Blockchain bc(1, test_data_dir);
    EXPECT_EQ(bc.getHeight(), 1); // Genesis block
}

TEST_F(BlockchainTest, ChainValidity) {
    Blockchain bc(1, test_data_dir);
    EXPECT_TRUE(bc.isChainValid());
}

TEST_F(BlockchainTest, MiningPendingTransactions) {
    Blockchain bc(1, test_data_dir);
    std::string miner = "miner_addr";
    
    // Initial balance
    EXPECT_EQ(bc.getBalance(miner), 0.0);
    
    bc.minePendingTransactions(miner);
    
    EXPECT_EQ(bc.getHeight(), 2);
    // Coinbase reward is usually added, need to check blockchain.cpp for reward value
    EXPECT_GT(bc.getBalance(miner), 0.0);
}
