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

void Blockchain::addBlock(const std::vector<Transaction> &txs) {
  for (const auto &tx : txs) {
    if (!isValidTransartion(tx)) {
      std::runtime_error("Invalid transaction detected");
    }
  }
  const Block &prev = getLatestBlock();
  Block newBlock(chain.size(), txs, prev.hash);

  newBlock.mine(difficulty);
  chain.push_back(newBlock);
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
