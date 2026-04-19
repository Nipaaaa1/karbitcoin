#include "core/blockchain.hpp"
#include "crypto/wallet.hpp"
#include "network/p2p_node.hpp"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

int main() {
    // Testing JSON dependency
    json test_json = {{"status", "networking_initialized"}};
    std::cout << "JSON Test: " << test_json.dump() << std::endl;

    // Testing P2PNode initialization
    karbitcoin::network::P2PNode node(8333);
    node.start();

    Blockchain blockchain(3);
    Wallet udin;
    Wallet jamal;

    blockchain.minePendingTransactions(udin.getAddress());

    Transaction t2 = createTransaction(udin.getAddress(), jamal.getAddress(), 20, 1.0,
                                        blockchain.getUtxoSet());
    udin.signTransaction(t2);

    blockchain.addTransaction({t2});
    blockchain.minePendingTransactions(jamal.getAddress());

    std::cout << "Address " << udin.getAddress()
                << " Balance: " << blockchain.getBalance(udin.getAddress()) << "\n";
    std::cout << "Address " << jamal.getAddress()
                << " Balance: " << blockchain.getBalance(jamal.getAddress())
                << "\n";

    std::cout << "Chain valid: " << blockchain.isChainValid() << "\n";

    node.stop();
    return 0;
}
