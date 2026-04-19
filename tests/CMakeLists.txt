find_package(GTest REQUIRED)
include(GoogleTest)

# Crypto Tests
add_executable(test_crypto 
    unit/crypto/test_hash.cpp
    unit/crypto/test_ecdsa.cpp
)
target_link_libraries(test_crypto 
    GTest::gtest_main 
    crypto
)
gtest_discover_tests(test_crypto)

# Core Tests
add_executable(test_core
    unit/core/test_transaction.cpp
    unit/core/test_block.cpp
    unit/core/test_blockchain.cpp
)
target_link_libraries(test_core
    GTest::gtest_main
    core
    crypto
)
gtest_discover_tests(test_core)
