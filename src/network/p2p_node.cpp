#include "network/p2p_node.hpp"
#include "network/serialization.hpp"
#include <iostream>

namespace karbitcoin::network {

// --- Session Implementation ---

Session::Session(tcp::socket socket, P2PNode& node) 
    : socket_(std::move(socket)), node_(node) {}

void Session::start() {
    do_read();
}

void Session::send(const std::string& message) {
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(message + "\n"),
        [this, self](boost::system::error_code ec, std::size_t /*length*/) {
            if (ec) {
                // Remove session on error (Phase 4 improvement)
            }
        });
}

void Session::do_read() {
    auto self(shared_from_this());
    boost::asio::async_read_until(socket_, buffer_, '\n',
        [this, self](boost::system::error_code ec, std::size_t length) {
            if (!ec) {
                std::istream is(&buffer_);
                std::string line;
                std::getline(is, line);
                
                if (!line.empty()) {
                    node_.handle_message(line, self);
                }
                
                do_read();
            }
        });
}

// --- P2PNode Implementation ---

P2PNode::P2PNode(unsigned short port, Blockchain& blockchain) 
    : port_(port), 
      blockchain_(blockchain),
      acceptor_(std::make_unique<tcp::acceptor>(io_context_, tcp::endpoint(tcp::v4(), port))) {}

void P2PNode::start() {
    std::cout << "P2P Node listening on port " << port_ << std::endl;
    do_accept();
    thread_ = std::thread([this]() { io_context_.run(); });
}

void P2PNode::stop() {
    io_context_.stop();
    if (thread_.joinable()) {
        thread_.join();
    }
}

void P2PNode::do_accept() {
    acceptor_->async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                auto session = std::make_shared<Session>(std::move(socket), *this);
                sessions_.push_back(session);
                session->start();
            }
            do_accept();
        });
}

void P2PNode::connect_to_peer(const std::string& host, unsigned short port) {
    tcp::resolver resolver(io_context_);
    auto endpoints = resolver.resolve(host, std::to_string(port));
    
    auto socket = std::make_shared<tcp::socket>(io_context_);
    boost::asio::async_connect(*socket, endpoints,
        [this, socket](boost::system::error_code ec, tcp::endpoint endpoint) {
            if (!ec) {
                auto session = std::make_shared<Session>(std::move(*socket), *this);
                sessions_.push_back(session);
                session->start();
            }
        });
}

void P2PNode::broadcast(const std::string& message) {
    for (auto& session : sessions_) {
        session->send(message);
    }
}

void P2PNode::handle_message(const std::string& raw_message, std::shared_ptr<Session> session) {
    try {
        nlohmann::json j = nlohmann::json::parse(raw_message);
        Message msg = j.get<Message>();

        switch (msg.type) {
            case MessageType::TRANSACTION: {
                Transaction tx = msg.payload.get<Transaction>();
                std::cout << "P2P: Received transaction " << tx.id << std::endl;
                // Add to mempool if valid
                try {
                    blockchain_.addTransaction(tx);
                    // Relay to others (Phase 4 propagation)
                    // broadcast(raw_message); // Simplified relay
                } catch (const std::exception& e) {
                    std::cerr << "P2P: Transaction invalid: " << e.what() << std::endl;
                }
                break;
            }
            case MessageType::BLOCK: {
                Block block = msg.payload.get<Block>();
                std::cout << "P2P: Received block " << block.hash << " at index " << block.index << std::endl;
                // Process block (Add to chain if valid)
                // TODO: Update Blockchain to support remote block adding
                break;
            }
            default:
                break;
        }
    } catch (const std::exception& e) {
        std::cerr << "P2P: Error handling message: " << e.what() << std::endl;
    }
}

void P2PNode::broadcast_transaction(const Transaction& tx) {
    Message msg = {MessageType::TRANSACTION, nlohmann::json(tx)};
    broadcast(nlohmann::json(msg).dump());
}

void P2PNode::broadcast_block(const Block& block) {
    Message msg = {MessageType::BLOCK, nlohmann::json(block)};
    broadcast(nlohmann::json(msg).dump());
}

} // namespace karbitcoin::network
