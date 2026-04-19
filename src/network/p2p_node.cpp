#include "network/p2p_node.hpp"
#include <iostream>

namespace karbitcoin::network {

// --- Session Implementation ---

Session::Session(tcp::socket socket) : socket_(std::move(socket)) {}

void Session::start() {
    do_read();
}

void Session::send(const std::string& message) {
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(message + "\n"),
        [this, self](boost::system::error_code ec, std::size_t /*length*/) {
            if (ec) {
                std::cerr << "Write error: " << ec.message() << std::endl;
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
                    std::cout << "Received message: " << line << std::endl;
                    // TODO: Handle message processing (Phase 4)
                }
                
                do_read();
            } else {
                if (ec != boost::asio::error::eof) {
                    std::cerr << "Read error: " << ec.message() << std::endl;
                }
            }
        });
}

// --- P2PNode Implementation ---

P2PNode::P2PNode(unsigned short port) 
    : port_(port), 
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
                std::cout << "Accepted connection from " << socket.remote_endpoint() << std::endl;
                auto session = std::make_shared<Session>(std::move(socket));
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
                std::cout << "Connected to peer: " << endpoint << std::endl;
                auto session = std::make_shared<Session>(std::move(*socket));
                sessions_.push_back(session);
                session->start();
            } else {
                std::cerr << "Connect error to " << endpoint << ": " << ec.message() << std::endl;
            }
        });
}

void P2PNode::broadcast(const std::string& message) {
    for (auto& session : sessions_) {
        session->send(message);
    }
}

} // namespace karbitcoin::network
