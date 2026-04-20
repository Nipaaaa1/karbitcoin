find_package(OpenSSL REQUIRED)

add_library(core  
    core/block.cpp  
    core/blockchain.cpp  
    core/transaction.cpp  
    core/storage.cpp
)

add_library(crypto  
    crypto/hash.cpp  
    crypto/ecdsa.cpp
    crypto/address.cpp
    crypto/wallet.cpp
    crypto/utxo.cpp
)

add_library(network
    network/p2p_node.cpp
)

target_link_libraries(crypto  
    OpenSSL::Crypto  
)

target_link_libraries(network
    core
    crypto
    ${Boost_LIBRARIES}
    nlohmann_json::nlohmann_json
)

add_executable(karbitcoin main.cpp)

target_link_libraries(karbitcoin  
    core  
    crypto
    network
)
