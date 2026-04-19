#pragma once

#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include <memory>
#include <set>

namespace karbitcoin::network {

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket);
    void start();
    void send(const std::string& message);

private:
    void do_read();
    void do_write();

    tcp::socket socket_;
    boost::asio::streambuf buffer_;
};

class P2PNode {
public:
    P2PNode(unsigned short port);
    void start();
    void stop();

    void connect_to_peer(const std::string& host, unsigned short port);
    void broadcast(const std::string& message);

private:
    void do_accept();

    unsigned short port_;
    boost::asio::io_context io_context_;
    std::unique_ptr<tcp::acceptor> acceptor_;
    std::vector<std::shared_ptr<Session>> sessions_;
    std::thread thread_;
};

} // namespace karbitcoin::network
