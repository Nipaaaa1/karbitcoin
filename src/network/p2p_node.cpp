#include "network/p2p_node.hpp"
#include "network/serialization.hpp"
#include <iostream>

namespace karbitcoin::network {

// --- Session Implementation ---

Session::Session(tcp::socket socket, P2PNode& node) 
    : socket_(std::move(socket)), node_(node) {}

void Session::start() {
    // Send initial handshake
    Message msg = {MessageType::HANDSHAKE, {{"height", node_.get_blockchain().getHeight()}}};
    send(nlohmann::json(msg).dump());
    do_read();
}

void Session::send(const std::string& message) {
    auto self(shared_from_this());
    boost::asio::post(socket_.get_executor(), [this, self, message]() {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(message + "\n");
        if (!write_in_progress) {
            do_write();
        }
    });
}

void Session::do_write() {
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(write_msgs_.front()),
        [this, self](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                write_msgs_.pop_front();
                if (!write_msgs_.empty()) {
                    do_write();
                }
            } else {
                // Connection lost or error
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
                {
                    std::lock_guard<std::mutex> lock(sessions_mutex_);
                    sessions_.push_back(session);
                }
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
                {
                    std::lock_guard<std::mutex> lock(sessions_mutex_);
                    sessions_.push_back(session);
                }
                session->start();
            }
        });
}

void P2PNode::broadcast(const std::string& message) {
    std::lock_guard<std::mutex> lock(sessions_mutex_);
    for (auto& session : sessions_) {
        session->send(message);
    }
}

void P2PNode::handle_message(const std::string& raw_message, std::shared_ptr<Session> session) {
    try {
        nlohmann::json j = nlohmann::json::parse(raw_message);
        Message msg = j.get<Message>();

        switch (msg.type) {
            case MessageType::HANDSHAKE: {
                size_t peer_height = msg.payload.at("height").get<size_t>();
                size_t my_height = blockchain_.getHeight();
                std::cout << "P2P: Handshake from peer. Peer height: " << peer_height << ", My height: " << my_height << std::endl;
                
                if (peer_height > my_height) {
                    std::cout << "P2P: Peer is ahead. Requesting blocks from " << my_height << std::endl;
                    Message req = {MessageType::GET_BLOCKS, {{"from_index", my_height}}};
                    session->send(nlohmann::json(req).dump());
                }
                break;
            }
            case MessageType::GET_BLOCKS: {
                size_t from_index = msg.payload.at("from_index").get<size_t>();
                std::cout << "P2P: Peer requested blocks from index " << from_index << std::endl;
                for (size_t i = from_index; i < blockchain_.getHeight(); ++i) {
                    Message block_msg = {MessageType::BLOCK, nlohmann::json(blockchain_.getBlock(i))};
                    session->send(nlohmann::json(block_msg).dump());
                }
                break;
            }
            case MessageType::TRANSACTION: {
                Transaction tx = msg.payload.get<Transaction>();
                try {
                    blockchain_.addTransaction(tx);
                } catch (const std::exception& e) {
                    std::cerr << "P2P Error: Failed to add transaction: " << e.what() << std::endl;
                }
                break;
            }
            case MessageType::BLOCK: {
                Block block = msg.payload.get<Block>();
                try {
                    if (block.index == blockchain_.getHeight()) {
                        blockchain_.addBlock(block);
                        std::cout << "P2P: Successfully synchronized block " << block.index << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "P2P Error: Failed to add block " << block.index << ": " << e.what() << std::endl;
                }
                break;
            }
            default:
                break;
        }
    } catch (const std::exception& e) {
        std::cerr << "P2P Error: Failed to handle message: " << e.what() << std::endl;
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
