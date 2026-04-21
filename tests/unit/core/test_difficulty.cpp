#include <gtest/gtest.h>
#include <filesystem>
#include "core/blockchain.hpp"

namespace fs = std::filesystem;

class BlockchainTest : public ::testing::Test {
protected:
    std::string test_data_dir = "test_diff_data";

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

    // Helper methods to access private members
    void addBlockToChain(Blockchain& bc, const Block& b) {
        bc.chain.push_back(b);
    }

    std::vector<Block>& getChain(Blockchain& bc) {
        return bc.chain;
    }

    int callGetAdjustedDifficulty(Blockchain& bc) {
        return bc.getAdjustedDifficulty();
    }
};

TEST_F(BlockchainTest, DifficultyAdjustmentIncrease) {
    int initialDiff = 3;
    Blockchain bc(initialDiff, test_data_dir);
    
    long startTime = getChain(bc)[0].timestamp;
    
    for (int i = 1; i < 10; ++i) {
        Block b(i, {}, getChain(bc).back().hash, initialDiff);
        // Simulate very fast mining (1 second per block)
        b.timestamp = startTime + i;
        addBlockToChain(bc, b);
    }
    
    EXPECT_EQ(getChain(bc).size(), 10);
    
    // Next block should trigger adjustment
    int nextDiff = callGetAdjustedDifficulty(bc);
    EXPECT_EQ(nextDiff, initialDiff + 1);
}

TEST_F(BlockchainTest, DifficultyAdjustmentDecrease) {
    int initialDiff = 3;
    Blockchain bc(initialDiff, test_data_dir);
    
    long startTime = getChain(bc)[0].timestamp;
    
    for (int i = 1; i < 10; ++i) {
        Block b(i, {}, getChain(bc).back().hash, initialDiff);
        // Simulate very slow mining (1 hour per block)
        b.timestamp = startTime + (i * 3600);
        addBlockToChain(bc, b);
    }
    
    EXPECT_EQ(getChain(bc).size(), 10);
    
    // Next block should trigger adjustment
    int nextDiff = callGetAdjustedDifficulty(bc);
    EXPECT_EQ(nextDiff, initialDiff - 1);
}

TEST_F(BlockchainTest, NoAdjustmentMidInterval) {
    int initialDiff = 3;
    Blockchain bc(initialDiff, test_data_dir);
    
    for (int i = 1; i < 5; ++i) {
        Block b(i, {}, getChain(bc).back().hash, initialDiff);
        addBlockToChain(bc, b);
    }
    
    EXPECT_EQ(getChain(bc).size(), 5);
    
    int nextDiff = callGetAdjustedDifficulty(bc);
    EXPECT_EQ(nextDiff, initialDiff);
}
