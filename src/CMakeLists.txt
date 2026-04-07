add_library(core
    core/block.cpp
    core/blockchain.cpp
)

add_library(crypto
    crypto/hash.cpp
)

add_executable(karbitcoin main.cpp)

target_link_libraries(karbitcoin
    core
    crypto
)

find_package(OpenSSL REQUIRED)

target_link_libraries(crypto
    OpenSSL::Crypto
)
