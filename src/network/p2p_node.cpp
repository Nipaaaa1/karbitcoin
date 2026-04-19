#include "network/p2p_node.hpp"
#include <iostream>

namespace karbitcoin::network {

P2PNode::P2PNode(unsigned short port) : port_(port) {}

void P2PNode::start() {
    std::cout << "Starting P2P Node on port " << port_ << "..." << std::endl;
}

void P2PNode::stop() {
    std::cout << "Stopping P2P Node..." << std::endl;
}

} // namespace karbitcoin::network
