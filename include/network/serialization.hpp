#pragma once

#include <nlohmann/json.hpp>
#include "core/block.hpp"
#include "core/transaction.hpp"
#include "crypto/utxo.hpp"

// Forward declarations of serialization functions to ensure visibility
inline void to_json(nlohmann::json& j, const TxIn& t);
inline void from_json(const nlohmann::json& j, TxIn& t);
inline void to_json(nlohmann::json& j, const TxOut& t);
inline void from_json(const nlohmann::json& j, TxOut& t);
inline void to_json(nlohmann::json& j, const Transaction& t);
inline void from_json(const nlohmann::json& j, Transaction& t);
inline void to_json(nlohmann::json& j, const Block& b);
inline void from_json(const nlohmann::json& j, Block& b);

// Implementation

inline void to_json(nlohmann::json& j, const TxIn& t) {
    j = nlohmann::json{{"prevTxId", t.prevTxId}, {"outputIndex", t.outputIndex}};
}
inline void from_json(const nlohmann::json& j, TxIn& t) {
    j.at("prevTxId").get_to(t.prevTxId);
    j.at("outputIndex").get_to(t.outputIndex);
}

inline void to_json(nlohmann::json& j, const TxOut& t) {
    j = nlohmann::json{{"address", t.address}, {"amount", t.amount}};
}
inline void from_json(const nlohmann::json& j, TxOut& t) {
    j.at("address").get_to(t.address);
    j.at("amount").get_to(t.amount);
}

inline void to_json(nlohmann::json& j, const Transaction& t) {
    j = nlohmann::json{
        {"id", t.id},
        {"timestamp", t.timestamp},
        {"inputs", t.inputs},
        {"outputs", t.outputs},
        {"signature", t.signature},
        {"publicKey", t.publicKey}
    };
}
inline void from_json(const nlohmann::json& j, Transaction& t) {
    j.at("inputs").get_to(t.inputs);
    j.at("outputs").get_to(t.outputs);
    t.id = j.at("id").get<std::string>();
    t.timestamp = j.at("timestamp").get<long>();
    t.signature = j.at("signature").get<std::string>();
    t.publicKey = j.at("publicKey").get<std::string>();
}

inline void to_json(nlohmann::json& j, const Block& b) {
    j = nlohmann::json{
        {"index", b.index},
        {"timestamp", b.timestamp},
        {"transactions", b.transactions},
        {"previousHash", b.previousHash},
        {"hash", b.hash},
        {"nonce", b.nonce}
    };
}
inline void from_json(const nlohmann::json& j, Block& b) {
    j.at("transactions").get_to(b.transactions);
    b.index = j.at("index").get<int>();
    b.timestamp = j.at("timestamp").get<long>();
    b.previousHash = j.at("previousHash").get<std::string>();
    b.hash = j.at("hash").get<std::string>();
    b.nonce = j.at("nonce").get<int>();
}
