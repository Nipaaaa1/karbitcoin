#include "core/blockchain.hpp"
#include "crypto/wallet.hpp"
#include "network/p2p_node.hpp"
#include "network/serialization.hpp"
#include "network/message.hpp"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;
using namespace karbitcoin::network;

int main() {
    Blockchain blockchain(3);
    Wallet udin;
    Wallet jamal;

    blockchain.minePendingTransactions(udin.getAddress());

    Transaction t2 = createTransaction(udin.getAddress(), jamal.getAddress(), 20, 1.0,
                                        blockchain.getUtxoSet());
    udin.signTransaction(t2);

    // Test Transaction Serialization
    json t_json = t2;
    Transaction t_deserialized = t_json.get<Transaction>();
    std::cout << "Transaction Serialization Test: " << (t2.id == t_deserialized.id ? "PASSED" : "FAILED") << std::endl;

    // Test Message with Transaction Payload
    Message msg = {MessageType::TRANSACTION, t_json};
    json msg_json = msg;
    Message msg_deserialized = msg_json.get<Message>();
    std::cout << "Message Serialization Test: " << (msg_deserialized.type == MessageType::TRANSACTION ? "PASSED" : "FAILED") << std::endl;

    blockchain.addTransaction({t2});
    blockchain.minePendingTransactions(jamal.getAddress());

    // Test Block Serialization
    Block last_block = blockchain.getChain().back();
    json b_json = last_block;
    Block b_deserialized = b_json.get<Block>();
    std::cout << "Block Serialization Test: " << (last_block.hash == b_deserialized.hash ? "PASSED" : "FAILED") << std::endl;

    std::cout << "Chain valid: " << blockchain.isChainValid() << "\n";

    return 0;
}
