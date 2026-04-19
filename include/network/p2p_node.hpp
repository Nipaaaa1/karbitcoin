#pragma once

#include <boost/asio.hpp>
#include <vector>
#include <string>
#include <memory>

namespace karbitcoin::network {

class P2PNode {
public:
    P2PNode(unsigned short port);
    void start();
    void stop();

private:
    unsigned short port_;
    boost::asio::io_context io_context_;
};

} // namespace karbitcoin::network
