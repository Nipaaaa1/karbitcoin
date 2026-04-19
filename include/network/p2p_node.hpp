#pragma once

#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include "core/blockchain.hpp"
#include "network/message.hpp"
#include <vector>
#include <string>
#include <memory>
#include <functional>

namespace karbitcoin::network {

using boost::asio::ip::tcp;

class P2PNode;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket, P2PNode& node);
    void start();
    void send(const std::string& message);

private:
    void do_read();

    tcp::socket socket_;
    P2PNode& node_;
    boost::asio::streambuf buffer_;
};

class P2PNode {
public:
    P2PNode(unsigned short port, Blockchain& blockchain);
    void start();
    void stop();

    void connect_to_peer(const std::string& host, unsigned short port);
    void broadcast(const std::string& message);
    
    // Message Handlers
    void handle_message(const std::string& raw_message, std::shared_ptr<Session> session);
    void broadcast_transaction(const Transaction& tx);
    void broadcast_block(const Block& block);

private:
    void do_accept();

    unsigned short port_;
    Blockchain& blockchain_;
    boost::asio::io_context io_context_;
    std::unique_ptr<tcp::acceptor> acceptor_;
    std::vector<std::shared_ptr<Session>> sessions_;
    std::thread thread_;
};

} // namespace karbitcoin::network
