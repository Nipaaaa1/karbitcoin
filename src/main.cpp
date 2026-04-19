#include "core/blockchain.hpp"
#include "crypto/wallet.hpp"
#include "network/p2p_node.hpp"
#include "network/serialization.hpp"
#include "network/message.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <chrono>
#include <thread>

using json = nlohmann::json;
using namespace karbitcoin::network;

int main() {
    // Start node 1
    P2PNode node1(8333);
    node1.start();

    // Small delay to ensure node1 is listening
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Connect node 1 to itself as a test
    node1.connect_to_peer("127.0.0.1", 8333);

    // Small delay to allow connection to establish
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Test broadcast
    json msg = {{"type", "test"}, {"data", "hello world"}};
    node1.broadcast(msg.dump());

    // Wait to see "Received message" in logs
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    node1.stop();
    return 0;
}
