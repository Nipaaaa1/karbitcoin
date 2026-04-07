#include "core/blockchain.hpp"
#include "core/block.hpp"
#include "core/transaction.hpp"
#include <iostream>
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

const std::vector<Block> &Blockchain::getChain() const { return chain; }
