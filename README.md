# Karbitcoin (Mini Cryptocurrency)

![C++](https://img.shields.io/badge/C%2B%2B-17-blue)
![CMake](https://img.shields.io/badge/CMake-3.16+-green)
![GTest](https://img.shields.io/badge/GTest-1.10+-red)
![License](https://img.shields.io/badge/License-MIT-yellow)

A simple yet structured implementation of a cryptocurrency built from scratch using C++.
This project focuses on understanding the fundamentals of blockchain, cryptography, and distributed systems.

---

## 🧠 Overview

This project implements a minimal but proper cryptocurrency system, including:

- Blockchain data structure
- Proof of Work (PoW)
- UTXO-based transaction model
- Digital signature using OpenSSL (ECDSA)
- P2P Networking (TCP-based)
- Automated Unit & Integration Testing

---

## 🏗️ Project Structure

```
karbitcoin/
├── CMakeLists.txt
├── src/                # Implementation files
│   ├── core/           # Blockchain, Block, Transaction, Storage logic
│   ├── crypto/         # Hashing, ECDSA, Wallet, UTXO
│   ├── network/        # P2P Node, Serialization
│   └── main.cpp
├── include/            # Header files
├── tests/              # Test suite (GTest)
│   ├── unit/           # Unit tests for components
│   └── integration/    # Integration tests (flow)
└── build/              # Build directory
```
---

## ⚙️ Features

✅ **Implemented**

- [x] Blockchain with linked blocks
- [x] SHA-256 hashing
- [x] Proof of Work (mining + difficulty)
- [x] UTXO-based transaction model
- [x] ECDSA digital signature (OpenSSL)
- [x] Address generation from public key
- [x] Transaction validation (Signature & UTXO checks)
- [x] Mempool (pending transactions)
- [x] Mining with block rewards & fees
- [x] Full chain validation
- [x] P2P node communication (TCP)
- [x] Transaction & Block propagation
- [x] Chain synchronization (Handshake + Sync)
- [x] **JSON-based Persistence (Blocks, UTXO, Metadata)**
- [x] **Automated Testing (25+ cases)**

---

## 🛠️ Build & Run

### Requirements
- **C++17** compiler
- **CMake** (>= 3.16)
- **OpenSSL** (libcrypto)
- **Boost** (Asio, System)
- **nlohmann_json**
- **GTest** (Google Test)

### Build
```bash
cmake -S . -B build
cmake --build build -j$(nproc)
```

### Run Node
```bash
./build/bin/karbitcoin
```

---

## 🧪 Testing

We use **Google Test** for verification.

```bash
cd build
ctest --output-on-failure
```

Tests include:
- `test_crypto`: Hash, ECDSA, UTXO, and Wallet logic.
- `test_core`: Transaction, Block, Blockchain integrity, and Persistence.
- `test_integration`: Full mining flow (Transaction -> Mining -> Balance check).

---

## 🧠 Design Philosophy

- Simplicity over completeness
- Readability over optimization
- Incremental learning approach
- **Robust Persistence**: JSON-based storage with auto-recovery support for UTXO sets from block history.

---

## ⚠️ Disclaimer

This project is for educational purposes only.
Do NOT use this implementation in production or for real financial systems.

---

## 📌 Roadmap

- [x] Core blockchain
- [x] Proof of Work
- [x] Transactions & UTXO
- [x] Digital signatures
- [x] Mempool & Validation
- [x] Networking (Basic P2P + Sync)
- [x] Testing Framework (GTest)
- [x] **Persistence (Save/Load to disk)**
- [ ] CLI Wallet interface
- [ ] Difficulty adjustment algorithm
- [ ] Multi-threaded mining

---

## 🤝 Contribution

Feel free to fork and experiment. This project is designed to be a learning playground.

---

## 📜 License

MIT License
