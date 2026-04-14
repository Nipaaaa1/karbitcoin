find_package(OpenSSL REQUIRED)

add_library(core  
    core/block.cpp  
    core/blockchain.cpp  
    core/transaction.cpp  
)

add_library(crypto  
    crypto/hash.cpp  
    crypto/ecdsa.cpp
    crypto/address.cpp
    crypto/wallet.cpp
    crypto/utxo.cpp
)

target_link_libraries(crypto  
    OpenSSL::Crypto  
)

add_executable(karbitcoin main.cpp)

target_link_libraries(karbitcoin  
    core  
    crypto  
)
