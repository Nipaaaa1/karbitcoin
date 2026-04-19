#include <gtest/gtest.h>
#include "core/transaction.hpp"
#include "crypto/utxo.hpp"

TEST(TransactionTest, HashConsistency) {
    std::vector<TxIn> inputs = {{"prev_id", 0}};
    std::vector<TxOut> outputs = {{"address1", 50.0}};
    Transaction tx(inputs, outputs);
    
    std::string h1 = tx.calculateHash();
    std::string h2 = tx.calculateHash();
    
    EXPECT_EQ(h1, h2);
    EXPECT_FALSE(h1.empty());
}

TEST(TransactionTest, SetId) {
    Transaction tx;
    tx.setId();
    EXPECT_FALSE(tx.id.empty());
}

TEST(TransactionTest, ValueOutCalculation) {
    std::vector<TxIn> inputs;
    std::vector<TxOut> outputs = {
        {"addr1", 10.0},
        {"addr2", 25.5}
    };
    Transaction tx(inputs, outputs);
    
    EXPECT_DOUBLE_EQ(tx.getValueOut(), 35.5);
}
