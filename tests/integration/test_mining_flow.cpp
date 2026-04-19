#include <gtest/gtest.h>
#include "core/blockchain.hpp"
#include "crypto/wallet.hpp"

TEST(IntegrationTest, MiningFlowTest) {
    // 1. Setup Blockchain with low difficulty
    Blockchain bc(1); 
    
    Wallet miner;
    Wallet alice;
    Wallet bob;
    
    // 2. Miner mines genesis/initial block to get some coins (reward)
    bc.minePendingTransactions(miner.getAddress());
    double minerBalance = bc.getBalance(miner.getAddress());
    EXPECT_GT(minerBalance, 0.0);
    
    // 3. Miner sends coins to Alice
    double sendAmount = 10.0;
    double fee = 1.0;
    
    // We need UTXOs for the transaction
    Transaction tx = createTransaction(miner.getAddress(), alice.getAddress(), sendAmount, fee, bc.getUtxoSet());
    miner.signTransaction(tx);
    
    bc.addTransaction(tx);
    
    // 4. Mine a block containing Alice's transaction
    bc.minePendingTransactions(bob.getAddress()); // Bob mines this one
    
    // 5. Verify balances
    EXPECT_DOUBLE_EQ(bc.getBalance(alice.getAddress()), sendAmount);
    // Miner balance = Initial reward - sendAmount - fee
    EXPECT_DOUBLE_EQ(bc.getBalance(miner.getAddress()), minerBalance - sendAmount - fee);
    // Bob (miner of 2nd block) gets reward + fee
    EXPECT_GT(bc.getBalance(bob.getAddress()), 0.0);
    
    // 6. Final validity check
    EXPECT_TRUE(bc.isChainValid());
    EXPECT_EQ(bc.getHeight(), 3); // Genesis + 1st mine + 2nd mine
}
