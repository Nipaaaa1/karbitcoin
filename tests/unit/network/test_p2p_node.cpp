#include <gtest/gtest.h>
#include "network/p2p_node.hpp"
#include "core/blockchain.hpp"
#include <filesystem>
#include <chrono>
#include <thread>

using namespace karbitcoin::network;

class P2PNodeTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::filesystem::remove_all("data_test_1");
        std::filesystem::remove_all("data_test_2");
    }

    void TearDown() override {
        std::filesystem::remove_all("data_test_1");
        std::filesystem::remove_all("data_test_2");
    }
};

TEST_F(P2PNodeTest, BlockSynchronizationTest) {
    Blockchain bc1(1, "data_test_1");
    Blockchain bc2(1, "data_test_2");

    // Mine a block on bc1
    bc1.minePendingTransactions("address1");
    ASSERT_EQ(bc1.getHeight(), 2);
    ASSERT_EQ(bc2.getHeight(), 1);

    P2PNode node1(9001, bc1);
    P2PNode node2(9002, bc2);

    node1.start();
    node2.start();

    // Connect node2 to node1
    node2.connect_to_peer("127.0.0.1", 9001);

    // Wait for synchronization
    int retries = 0;
    while (bc2.getHeight() < 2 && retries < 50) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        retries++;
    }

    EXPECT_EQ(bc2.getHeight(), 2);
    EXPECT_EQ(bc2.getBlock(1).hash, bc1.getBlock(1).hash);

    node1.stop();
    node2.stop();
}

TEST_F(P2PNodeTest, RapidMessagesTest) {
    Blockchain bc1(1, "data_test_1");
    Blockchain bc2(1, "data_test_2");

    // Mine several blocks on bc1
    for (int i = 0; i < 5; ++i) {
        bc1.minePendingTransactions("address1");
    }
    ASSERT_EQ(bc1.getHeight(), 6);

    P2PNode node1(9003, bc1);
    P2PNode node2(9004, bc2);

    node1.start();
    node2.start();

    // Connect node2 to node1
    node2.connect_to_peer("127.0.0.1", 9003);

    // Wait for synchronization of all blocks
    int retries = 0;
    while (bc2.getHeight() < 6 && retries < 100) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        retries++;
    }

    EXPECT_EQ(bc2.getHeight(), 6);
    
    node1.stop();
    node2.stop();
}
