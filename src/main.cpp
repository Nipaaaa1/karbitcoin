#include "core/blockchain.hpp"
#include "crypto/wallet.hpp"
#include "network/p2p_node.hpp"
#include "network/serialization.hpp"
#include <iostream>
#include <chrono>
#include <thread>

using namespace karbitcoin::network;

int main() {
    Blockchain bc1(2); // Node 1
    Blockchain bc2(2); // Node 2

    // Node 1 mines a block first
    Wallet miner;
    bc1.minePendingTransactions(miner.getAddress());
    std::cout << "Node 1 height: " << bc1.getHeight() << std::endl;
    std::cout << "Node 2 height: " << bc2.getHeight() << std::endl;

    P2PNode node1(8333, bc1);
    P2PNode node2(8334, bc2);

    node1.start();
    node2.start();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Connect node 2 to node 1
    std::cout << "Node 2 connecting to Node 1..." << std::endl;
    node2.connect_to_peer("127.0.0.1", 8333);

    // Wait for synchronization
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "FINAL Node 1 height: " << bc1.getHeight() << std::endl;
    std::cout << "FINAL Node 2 height: " << bc2.getHeight() << std::endl;

    node1.stop();
    node2.stop();

    return 0;
}
