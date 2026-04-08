#include "core/blockchain.hpp"
#include "core/block.hpp"
#include "core/transaction.hpp"
#include "crypto/address.hpp"
#include "crypto/ecdsa.hpp"
#include <stdexcept>
#include <string>
#include <vector>

Blockchain::Blockchain(int diff) : difficulty(diff) {
  Block genesisBlock = createGenesisBlock();
  genesisBlock.mine(difficulty);
  chain.push_back(genesisBlock);
}

Block Blockchain::createGenesisBlock() {
  return Block(0, std::vector<Transaction>{}, "0");
}

const Block &Blockchain::getLatestBlock() const { return chain.back(); }

void Blockchain::addTransaction(const Transaction &tx) {
  if (!isValidTransartion(tx)) {
    std::runtime_error("Invalid transaction detected");
  }

  double balance = getBalance(tx.from);

  for (const auto &t : mempool) {
    if (tx.from == t.from) {
      balance -= t.amount;
    }
  }

  if (balance < 0) {
    std::runtime_error("Invalid transaction (double spending) detected");
  }

  mempool.push_back(tx);
}

void Blockchain::minePendingTransactions(const std::string &minerAddress) {
  Transaction reward("SYSTEM", minerAddress, 50);
  mempool.push_back(reward);

  const Block &prev = getLatestBlock();
  Block newBlock(chain.size(), mempool, prev.hash);

  newBlock.mine(difficulty);
  chain.push_back(newBlock);

  mempool.clear();
}

double Blockchain::getBalance(const std::string &address) const {
  double balance = 0;

  for (const auto &block : chain) {
    for (const auto &tx : block.transactions) {
      if (tx.from == address) {
        balance -= tx.amount;
      }

      if (tx.to == address) {
        balance += tx.amount;
      }
    }
  }

  return balance;
}

bool Blockchain::isValidTransartion(const Transaction &tx) const {
  if (tx.from == "SYSTEM")
    return true;

  if (!verifySignature(tx.publicKey, tx.calculateHash(), tx.signature)) {
    return false;
  }

  std::string derived = publicKeyToAddress(tx.publicKey);
  if (derived != tx.from) {
    return false;
  }

  if (getBalance(tx.from) < tx.amount) {
    return false;
  }

  return true;
}

const std::vector<Block> &Blockchain::getChain() const { return chain; }
