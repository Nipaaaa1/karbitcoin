#include <gtest/gtest.h>
#include <filesystem>
#include "core/blockchain.hpp"
#include "core/storage.hpp"

namespace fs = std::filesystem;

class PersistenceTest : public ::testing::Test {
protected:
    std::string data_dir = "test_blockchain_data";

    void SetUp() override {
        if (fs::exists(data_dir)) {
            fs::remove_all(data_dir);
        }
    }

    void TearDown() override {
        if (fs::exists(data_dir)) {
            fs::remove_all(data_dir);
        }
    }
};

TEST_F(PersistenceTest, SaveAndLoadBlockchain) {
    std::string miner = "miner_addr";
    {
        // Create a blockchain and mine a block
        Blockchain bc(1, data_dir); // difficulty 1
        bc.minePendingTransactions(miner);
        EXPECT_EQ(bc.getHeight(), 2);
        EXPECT_TRUE(fs::exists(data_dir + "/blocks/block_0.json"));
        EXPECT_TRUE(fs::exists(data_dir + "/blocks/block_1.json"));
    }

    {
        // Reload the blockchain from disk
        Blockchain bc_reloaded(1, data_dir);
        EXPECT_EQ(bc_reloaded.getHeight(), 2);
        EXPECT_TRUE(bc_reloaded.isChainValid());
        EXPECT_GT(bc_reloaded.getBalance(miner), 0.0);
    }
}
