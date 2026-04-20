#include "core/blockchain.hpp"
#include "crypto/wallet.hpp"
#include "network/p2p_node.hpp"
#include "network/serialization.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>

using namespace karbitcoin::network;

void print_help() {
    std::cout << "\nKarbitcoin CLI Commands:\n"
              << "  help                       - Show this help message\n"
              << "  status                     - Show node and wallet status\n"
              << "  balance                    - Show wallet balance\n"
              << "  mine                       - Mine pending transactions\n"
              << "  send <address> <amount>    - Send coins to an address\n"
              << "  connect <ip> <port>        - Connect to a peer\n"
              << "  info                       - Show blockchain info\n"
              << "  exit                       - Exit the application\n" << std::endl;
}

int main(int argc, char* argv[]) {
    unsigned short port = 8333;
    if (argc > 1) {
        port = static_cast<unsigned short>(std::stoi(argv[1]));
    }

    std::string dataDir = "data_" + std::to_string(port);
    Blockchain bc(2, dataDir);
    
    Wallet wallet;
    std::string walletFile = dataDir + "/wallet.json";
    if (std::filesystem::exists(walletFile)) {
        wallet.loadFromFile(walletFile);
        std::cout << "Loaded wallet from " << walletFile << std::endl;
    } else {
        std::filesystem::create_directories(dataDir);
        wallet.saveToFile(walletFile);
        std::cout << "Created new wallet at " << walletFile << std::endl;
    }

    P2PNode node(port, bc);
    node.start();

    std::cout << "Karbitcoin Node started on port " << port << std::endl;
    std::cout << "Wallet Address: " << wallet.getAddress() << std::endl;
    print_help();

    std::string line;
    while (true) {
        std::cout << "karbitcoin> ";
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;

        if (cmd == "exit") {
            break;
        } else if (cmd == "help") {
            print_help();
        } else if (cmd == "status") {
            std::cout << "Node Status:\n"
                      << "  Port: " << port << "\n"
                      << "  Address: " << wallet.getAddress() << "\n"
                      << "  Balance: " << bc.getBalance(wallet.getAddress()) << " KBC\n"
                      << "  Chain Height: " << bc.getHeight() << std::endl;
        } else if (cmd == "balance") {
            std::cout << "Balance: " << bc.getBalance(wallet.getAddress()) << " KBC" << std::endl;
        } else if (cmd == "info") {
            std::cout << "Blockchain Info:\n"
                      << "  Height: " << bc.getHeight() << "\n"
                      << "  Difficulty: 2\n"
                      << "  Data Dir: " << dataDir << std::endl;
        } else if (cmd == "connect") {
            std::string ip;
            unsigned short peerPort;
            if (ss >> ip >> peerPort) {
                node.connect_to_peer(ip, peerPort);
                std::cout << "Connecting to peer " << ip << ":" << peerPort << "..." << std::endl;
            } else {
                std::cout << "Usage: connect <ip> <port>" << std::endl;
            }
        } else if (cmd == "mine") {
            std::cout << "Mining..." << std::endl;
            bc.minePendingTransactions(wallet.getAddress());
            node.broadcast_block(bc.getBlock(bc.getHeight() - 1));
            std::cout << "Block mined! New height: " << bc.getHeight() << std::endl;
        } else if (cmd == "send") {
            std::string toAddress;
            double amount;
            if (ss >> toAddress >> amount) {
                try {
                    double fee = 0.1; // Default fee
                    Transaction tx = createTransaction(wallet.getAddress(), toAddress, amount, fee, bc.getUtxoSet());
                    wallet.signTransaction(tx);
                    bc.addTransaction(tx);
                    node.broadcast_transaction(tx);
                    std::cout << "Transaction sent! ID: " << tx.id << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
            } else {
                std::cout << "Usage: send <address> <amount>" << std::endl;
            }
        } else {
            std::cout << "Unknown command. Type 'help' for available commands." << std::endl;
        }
    }

    node.stop();
    return 0;
}
