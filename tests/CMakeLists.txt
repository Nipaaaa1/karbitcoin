find_package(GTest REQUIRED)
include(GoogleTest)

# Crypto Tests
add_executable(test_crypto 
    unit/crypto/test_hash.cpp
    unit/crypto/test_ecdsa.cpp
    unit/crypto/test_utxo.cpp
    unit/crypto/test_wallet.cpp
)
target_link_libraries(test_crypto 
    GTest::gtest_main 
    crypto
    core
)
gtest_discover_tests(test_crypto)

# Core Tests
add_executable(test_core
    unit/core/test_transaction.cpp
    unit/core/test_block.cpp
    unit/core/test_blockchain.cpp
    unit/core/test_storage.cpp
    unit/core/test_persistence.cpp
)
target_link_libraries(test_core
    GTest::gtest_main
    core
    crypto
)
gtest_discover_tests(test_core)

# Integration Tests
add_executable(test_integration
    integration/test_mining_flow.cpp
)
target_link_libraries(test_integration
    GTest::gtest_main
    core
    crypto
)
gtest_discover_tests(test_integration)
