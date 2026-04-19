#pragma once

namespace karbitcoin::network {
    enum class MessageType {
        HANDSHAKE,
        TRANSACTION,
        BLOCK,
        GET_BLOCKS,
        INVENTORY
    };
}
