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

TEST_F(PersistenceTest, UtxoPersistence) {
    std::string miner = "miner_addr";
    double initial_balance = 0;
    {
        Blockchain bc(1, data_dir);
        bc.minePendingTransactions(miner);
        initial_balance = bc.getBalance(miner);
        EXPECT_GT(initial_balance, 0);
        // UTXO should be saved after block is mined (via applyTransaction)
    }

    {
        // Restart blockchain
        Blockchain bc_reloaded(1, data_dir);
        // Balance should be same
        EXPECT_EQ(bc_reloaded.getBalance(miner), initial_balance);
    }
}

TEST_F(PersistenceTest, MetadataPersistence) {
    {
        Blockchain bc(3, data_dir); // Difficulty 3
        bc.minePendingTransactions("miner");
        EXPECT_EQ(bc.getHeight(), 2);
    }

    {
        Blockchain bc_reloaded(1, data_dir); // Constructor diff 1 should be overridden by saved 3
        // We need to check if difficulty is actually used in some way we can verify
        // For now, let's just check if it was loaded
        bc_reloaded.minePendingTransactions("miner2");
        EXPECT_EQ(bc_reloaded.getHeight(), 3);
        
        // Verify the last block mined with difficulty 3
        std::string target(3, '0');
        EXPECT_EQ(bc_reloaded.getBlock(2).hash.substr(0, 3), target);
    }
}
