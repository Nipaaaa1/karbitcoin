# Karbitcoin (Mini Cryptocurrency)

![C++](https://img.shields.io/badge/C%2B%2B-17-blue)
![CMake](https://img.shields.io/badge/CMake-3.16+-green)
![License](https://img.shields.io/badge/License-MIT-yellow)

A simple yet structured implementation of a cryptocurrency built from scratch using C++.
This project focuses on understanding the fundamentals of blockchain, cryptography, and distributed systems.

---

## 🧠 Overview

This project implements a minimal but proper cryptocurrency system, including:

- Blockchain data structure
- Proof of Work (PoW)
- Transaction system (account-based)
- Digital signature using OpenSSL (ECDSA)
- Mempool (transaction pool)
- Block & chain validation

The goal is not to replicate Bitcoin fully, but to build a clean, understandable, and extensible foundation.

---

## 🏗️ Project Structure

```
karbitcoin/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── core/
│   │   ├── block.cpp
│   │   ├── blockchain.cpp
│   │   ├── transaction.cpp
│   ├── crypto/
│   │   ├── hash.cpp
│   │   ├── ecdsa.cpp
│   │   ├── address.cpp
├── include/
│   ├── core/
│   │   ├── block.hpp
│   │   ├── blockchain.hpp
│   │   ├── transaction.hpp
│   ├── crypto/
│   │   ├── hash.hpp
│   │   ├── ecdsa.hpp
│   │   ├── address.hpp
├── data/
```
---

## ⚙️ Features

✅ Implemented

- [x] Blockchain with linked blocks
- [x] SHA-256 hashing
- [x] Proof of Work (mining + difficulty)
- [x] UTXO-based transaction model (fixed selection logic)
- [x] Proper Transaction ID initialization
- [x] Balance calculation from UTXO set
- [x] ECDSA digital signature (OpenSSL)
- [x] Address generation from public key
- [x] Transaction validation:
  - Signature verification
  - UTXO availability check (Double-spending protection)
  - Sender authenticity
- [x] Mempool (pending transactions)
- [x] Mining with block rewards (Coinbase transactions)
- [x] Block validation (hash, PoW, linkage)
- [x] Full chain validation (tamper detection)
- [x] Transaction fees (incentive for miners)
- [x] Replay attack protection (unique timestamps in transactions)
- [x] Advanced double-spending prevention across unconfirmed blocks (mempool validation)
- [x] P2P node communication (TCP-based)
- [x] Transaction broadcast
- [x] Block propagation
- [x] Chain synchronization (Basic handshake + sync)

---

❌ Not Yet Implemented

🔐 Security & Validation

- [ ] (Completed)

🌐 Networking

- [ ] Peer discovery (Automatic peer sharing)

⚡ Performance

- [ ] Multi-threaded mining
- [ ] Difficulty adjustment algorithm
- [ ] Block size limit
- [ ] Transaction prioritization

💾 Persistence

- [ ] Save blockchain to disk
- [ ] Load blockchain on startup
- [ ] Mempool persistence

👛 Wallet & CLI

- [ ] CLI commands (send, balance, mine)
- [ ] Wallet file management
- [ ] Secure private key storage

🧠 Advanced Features

- [ ] Smart contracts
- [ ] Proof of Stake (PoS)
- [ ] Token economics (halving, inflation control)
- [ ] Merkle Tree for transactions

---

## 🛠️ Build & Run

Requirements

- C++17+
- CMake (>= 3.16)
- OpenSSL

Build
```
cmake -S . -B build
cmake --build build
```
Run

`./build/bin/karbitcoin`

---

## 🧪 Example Flow

1. Generate wallet (keypair)
2. Create transaction
3. Sign transaction
4. Add to mempool
5. Mine block
6. Validate chain

---

## 🧠 Design Philosophy

This project follows:

- Simplicity over completeness
- Readability over optimization
- Incremental learning approach

---

## ⚠️ Disclaimer

This project is for educational purposes only.
Do NOT use this implementation in production or for real financial systems.

---

## 📌 Roadmap

- [x] Core blockchain
- [x] Proof of Work
- [x] Transactions
- [x] Digital signatures
- [x] Mempool
- [x] Validation
- [x] Networking (Basic P2P + Sync)
- [ ] Persistence (next step)
- [ ] CLI wallet

---

## 🤝 Contribution

Feel free to fork and experiment.
This project is designed to be a learning playground.

---

## 📜 License

MIT License
