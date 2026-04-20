#include <gtest/gtest.h>
#include "crypto/wallet.hpp"
#include "crypto/ecdsa.hpp"
#include <cstdio>

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

TEST(WalletTest, PersistenceTest) {
    std::string testFile = "test_wallet.json";
    Wallet wallet;
    std::string originalAddress = wallet.getAddress();
    std::string originalPriv = wallet.getPrivateKey();
    std::string originalPub = wallet.getPublicKey();

    wallet.saveToFile(testFile);

    Wallet loadedWallet;
    loadedWallet.loadFromFile(testFile);

    EXPECT_EQ(loadedWallet.getAddress(), originalAddress);
    EXPECT_EQ(loadedWallet.getPrivateKey(), originalPriv);
    EXPECT_EQ(loadedWallet.getPublicKey(), originalPub);

    std::remove(testFile.c_str());
}
