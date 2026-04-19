#include "core/blockchain.hpp"
#include "crypto/wallet.hpp"
#include "network/p2p_node.hpp"
#include "network/serialization.hpp"
#include <iostream>
#include <chrono>
#include <thread>

using namespace karbitcoin::network;

int main() {
    Blockchain bc1(2);
    Blockchain bc2(2);

    P2PNode node1(8333, bc1);
    P2PNode node2(8334, bc2);

    node1.start();
    node2.start();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Connect node 2 to node 1
    node2.connect_to_peer("127.0.0.1", 8333);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Create a wallet and transaction
    Wallet udin;
    Wallet jamal;
    
    // Give some balance to udin in bc1 and bc2 via mining (simulating sync or initial state)
    bc1.minePendingTransactions(udin.getAddress());
    bc2.minePendingTransactions(udin.getAddress());

    Transaction tx = createTransaction(udin.getAddress(), jamal.getAddress(), 10, 1.0, bc1.getUtxoSet());
    udin.signTransaction(tx);

    std::cout << "Node 1 broadcasting transaction..." << std::endl;
    node1.broadcast_transaction(tx);

    // Wait for propagation
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    node1.stop();
    node2.stop();

    return 0;
}
