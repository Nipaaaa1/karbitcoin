#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace karbitcoin::network {

enum class MessageType {
    HANDSHAKE,
    TRANSACTION,
    BLOCK,
    GET_BLOCKS,
    INVENTORY
};

struct Message {
    MessageType type;
    nlohmann::json payload;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Message, type, payload)
};

} // namespace karbitcoin::network
