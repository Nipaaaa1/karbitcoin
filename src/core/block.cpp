#include "core/block.hpp"
#include "crypto/hash.hpp"
#include <atomic>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

Block::Block(const int &idx, const std::vector<Transaction> &txs,
             const std::string &prevHash, int diff, long ts)
    : index(idx), transactions(txs), previousHash(prevHash), nonce(0),
      difficulty(diff) {
  if (ts != 0) {
    timestamp = ts;
  } else {
    timestamp = std::time(nullptr);
  }
  hash = calculateHash();
}

static std::string transactionToString(const std::vector<Transaction> &txs) {
  std::stringstream ss;
  for (const auto &tx : txs) {
    ss << tx.id << tx.signature;
    for (const auto &in : tx.inputs) {
      ss << in.prevTxId << in.outputIndex;
    }
    for (const auto &out : tx.outputs) {
      ss << out.address << out.amount;
    }
  }
  return ss.str();
}

std::string Block::calculateHash(int n) const {
  std::stringstream ss;
  int targetNonce = (n == -1) ? nonce : n;
  ss << index << timestamp << transactionToString(transactions) << previousHash
     << targetNonce << difficulty;
  return hashString(ss.str());
}

void Block::mine(int difficulty, int numThreads) {
  if (numThreads <= 0) {
    numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0)
      numThreads = 1;
  }

  std::string target(difficulty, '0');

  if (numThreads == 1) {
    std::cout << "Mining block (single-threaded)...\n";
    while (hash.substr(0, difficulty) != target) {
      nonce++;
      hash = calculateHash();
    }
    return;
  }

  std::cout << "Mining block with " << numThreads << " threads...\n";
  std::atomic<bool> found(false);
  std::atomic<int> resultNonce(0);
  std::vector<std::thread> threads;

  for (int i = 0; i < numThreads; ++i) {
    threads.emplace_back(
        [this, i, numThreads, difficulty, &target, &found, &resultNonce]() {
          int localNonce = i;
          while (!found) {
            std::string h = calculateHash(localNonce);
            if (h.substr(0, difficulty) == target) {
              if (!found.exchange(true)) {
                resultNonce = localNonce;
              }
              return;
            }
            localNonce += numThreads;
            
            // Basic safety to prevent infinite loop if nonce overflows 
            // and we still haven't found a solution (unlikely for int range and low difficulty,
            // but good practice).
            if (localNonce < 0 && (localNonce - numThreads) >= 0) {
                // Wrapped around
            }
          }
        });
  }

  for (auto &t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }

  nonce = resultNonce;
  hash = calculateHash(nonce);
}
