#include <gtest/gtest.h>
#include "crypto/wallet.hpp"
#include "crypto/ecdsa.hpp"

TEST(WalletTest, Initialization) {
    Wallet wallet;
    EXPECT_FALSE(wallet.getAddress().empty());
    EXPECT_FALSE(wallet.getPublicKey().empty());
}

TEST(WalletTest, SignTransaction) {
    Wallet wallet;
    Transaction tx;
    tx.inputs = {{"prev", 0}};
    tx.outputs = {{"target", 10.0}};
    
    wallet.signTransaction(tx);
    
    EXPECT_FALSE(tx.signature.empty());
    EXPECT_EQ(tx.publicKey, wallet.getPublicKey());
    
    EXPECT_TRUE(verifySignature(tx.publicKey, tx.calculateHash(), tx.signature));
}
