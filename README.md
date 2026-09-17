# 🏦 NexusBank Core — Enterprise Console Banking & ATM Engine

<div align="center">

[![C++ Standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg?style=for-the-badge&logo=c%2B%2B)](https://en.cppreference.com/w/cpp/17)
[![OOP Architecture](https://img.shields.io/badge/Architecture-Object--Oriented-orange?style=for-the-badge&logo=codeforces)](https://github.com/Abdullah-Shalgam/NexusBank-Core-OOP-Engine)
[![Build Engine](https://img.shields.io/badge/Build-CMake%20v3.16+-brightgreen.svg?style=for-the-badge&logo=cmake)](https://cmake.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg?style=for-the-badge)](https://github.com/Abdullah-Shalgam/NexusBank-Core-OOP-Engine)
[![License](https://img.shields.io/badge/License-MIT-green.svg?style=for-the-badge)](LICENSE)

<p align="center">
  <b>NexusBank Core</b> is a robust, modular, educational-focused console banking and foreign exchange (FX) transactional platform engineered with <b>Modern C++17</b>. Demonstrating production-grade Object-Oriented Design (OOD), bitmask-driven Role-Based Access Control (RBAC), multi-layered data serialization, and ACID-inspired transaction safety.
</p>

</div>

---

## 🗂️ Table of Contents

1. [Architectural Overview](#%EF%B8%8F-architectural-overview)
2. [Domain Models & Data Flow](#-domain-models--data-flow)
3. [Deep-Dive Engineering & Design Patterns](#-deep-dive-engineering--design-patterns)
4. [Functional Modules](#-functional-modules)
5. [Data Layout & Serialization Specification](#-data-layout--serialization-specification)
6. [Prerequisites & Supported Toolchains](#-prerequisites--supported-toolchains)
7. [Comprehensive Build & Execution Guide](#-comprehensive-build--execution-guide)
8. [Repository Structure](#-repository-structure)
9. [Default Credentials & Initial State](#-default-credentials--initial-state)
10. [Contact & Developer Info](#-contact--developer-info)

---

## 🏛️ Architectural Overview

NexusBank Core adopts an enterprise **N-Tier Layered Architecture**, establishing strict decoupling between presentation components, domain logic, and file-based persistence.

```text
┌─────────────────────────────────────────────────────────────────────────┐
│                         PRESENTATION LAYER (UI)                         │
│  clsLoginScreen | clsMainScreen | clsTransactionsScreen | clsFXScreens  │
└────────────────────────────────────┬────────────────────────────────────┘
                                     │ Invokes UI Workflows & Renderers
                                     ▼
┌─────────────────────────────────────────────────────────────────────────┐
│                       DOMAIN CORE & BUSINESS LOGIC                      │
│     clsPerson (Base) ◄── clsBankClient, clsUser | clsCurrency           │
│     State Management: Active Session, Rollback Handlers, Bitmasks       │
└────────────────────────────────────┬────────────────────────────────────┘
                                     │ Uses Utilities & File Converters
                                     ▼
┌─────────────────────────────────────────────────────────────────────────┐
│                    PERSISTENCE & SYSTEM LIBRARIES (Lib)                 │
│  Flat-File Engine (Clients, Users, FX) | DateLib | UtilLib | StringLib  │
│  Security Layer: Reversible Cipher / PIN & Password Masking / Sanitizer │
└─────────────────────────────────────────────────────────────────────────┘
```

### ✨ Key Architectural Strengths:
* **Clean Interface Segregation:** Base screen classes (`clsScreen`, `clsUserScreenBase`, `clsClientScreenBase`, `clsCurrencyScreenBase`) abstract styling, headers, audit footprints, and UI authorization filters.
* **Const-Correct & Copy-Free Semantics:** Minimizes memory footprint and cache invalidation by passing models, string buffers, and collections via `const T&`.
* **Zero Global Pollution:** Standard header namespaces remain cleanly scoped, and domain entities manage their own record (de)serialization.

---

## 🔄 Domain Models & Data Flow

### Class Inheritance Structure

```text
               ┌────────────────────────┐
               │        clsPerson       │
               │ - FirstName, LastName  │
               │ - Email, Phone         │
               └───────────┬────────────┘
                           │
             ┌─────────────┴─────────────┐
             ▼                           ▼
  ┌───────────────────────┐   ┌───────────────────────┐
  │     clsBankClient     │   │        clsUser        │
  │ - AccountNumber       │   │ - UserName            │
  │ - PinCode (Encrypted) │   │ - Password (Encrypted)│
  │ - AccountBalance      │   │ - Permissions Bitmask │
  └───────────────────────┘   └───────────────────────┘
```

### Persistence Pipeline

```text
[Disk Flat-File]
       │ (std::fstream read)
       ▼
[_LoadDataFromFile()] ──> [_ConvertLineToObject()] ──> [Decrypt Cipher] ──> In-Memory Model Object
                                                                                 │
                                                                        (Business Mutation)
                                                                                 │
[Persistent Disk Flush] ◄── [Encrypt Cipher] ◄── [_ConvertObjectToLine()] ◄────────┘
```

---

## 🎯 Deep-Dive Engineering & Design Patterns

### 1️⃣ Bitwise Access Control (RBAC Engine)

Permissions are allocated using binary flags evaluated via Bitwise-AND (`&`) and Bitwise-OR (`|`) operations, yielding constant-time $\mathcal{O}(1)$ authorization checks:

```cpp
enum enMainMenuPermissions {
    eAll               = -1,   // 11111111...
    pListClients       = 1,    // 00000001
    pAddNewClient      = 2,    // 00000010
    pDeleteClient      = 4,    // 00000100
    pUpdateClients     = 8,    // 00001000
    pFindClient        = 16,   // 00010000
    pTransactions      = 32,   // 00100000
    pManageUsers       = 64,   // 01000000
    pLoginRegister     = 128,  // 10000000
    pCurrencyExchange  = 256   // 000100000000
};
```

### 2️⃣ Transaction Integrity & Rollback Protection

To guarantee financial consistency during fund transfers:
1. Sender balance is debited (`Withdraw(Amount)`).
2. Recipient balance is credited (`DestinationClient.Deposit(Amount)`).
3. If destination crediting fails, the engine instantly re-credits the sender balance to eliminate state divergence.
4. An immutable audit log entry is written to `TransferLog.txt`.

### 3️⃣ Masked Stream Input & Security Checkpoints

* Passwords and security PINs are captured char-by-char via direct console hooks with backspace handling and star-masking (`*`).
* High-risk operations (Client deletion, User permission modification, FX rate reconfiguration) require Active Session Password Re-Authentication.

---

## 🚀 Functional Modules

| Module | Core Responsibility | Capabilities |
| :--- | :--- | :--- |
| **🔐 Authentication & IAM** | Identity & Session Management | Max login lockouts (3 attempts), credential decryption, permission bitmasking. |
| **👥 Client Management** | Bank Accounts Ledger | Full CRUD lifecycle, unique account assertion, selective field updates. |
| **💳 Transaction Core** | Financial Operations | Deposit, withdrawal validation, account-to-account transfer with rollback, total balance aggregation. |
| **💱 FX & Currency Engine** | Multi-Currency Conversion | Base-currency conversions (USD Anchor), dynamic global formatting, real-time rate updates. |
| **📜 Audit Logs & Forensics** | Compliance & Traceability | Comprehensive transfer register (`TransferLog.txt`) and authentication timeline (`LoginRegister.txt`). |

---

## 💾 Data Layout & Serialization Specification

Data is persisted in standard flat-files using `#//#` as the field separator.

### 1. `Clients.txt` (Client Ledgers)
```text
FirstName#//#LastName#//#Email#//#Phone#//#AccountNumber#//#EncryptedPin#//#AccountBalance
```

### 2. `Users.txt` (System Operators & IAM)
```text
FirstName#//#LastName#//#Email#//#Phone#//#UserName#//#EncryptedPassword#//#PermissionsInteger
```

### 3. `Currencies.txt` (Foreign Exchange Rates)
```text
Country#//#CurrencyCode#//#CurrencyName#//#RateRelativeToUSD
```

### 4. `TransferLog.txt` (Audit Ledger)
```text
DateTime#//#SenderAcc#//#DestAcc#//#Amount#//#SenderBalAfter#//#DestBalAfter#//#OperatorUserName
```

---

## 🛠️ Prerequisites & Supported Toolchains

| Operating System | Compiler Compatibility | Build Automation |
| :--- | :--- | :--- |
| **Windows 10 / 11** | MSVC 2019+, MinGW-w64 (GCC 9+), Clang 10+ | CMake v3.16+ |
| **Linux (Ubuntu, Debian, Fedora)** | GCC 9+, Clang 10+ | CMake v3.16+, Make, Ninja |
| **macOS (Intel & Apple Silicon)** | Apple Clang 11+, GCC 10+ | CMake v3.16+ |

---

## 🔨 Comprehensive Build & Execution Guide

### 📦 1. Clone the Repository

```bash
git clone https://github.com/Abdullah-Shalgam/NexusBank-Core-OOP-Engine.git
cd NexusBank-Core-OOP-Engine
```

### 🪟 2. Building on Windows (MinGW / PowerShell)

```powershell
# Generate build tree
cmake -B build -G "MinGW Makefiles"

# Compile target
cmake --build build --config Release

# Navigate and Launch Executable
cd bin
.\BankSystem.exe
```

### 🐧 3. Building on Linux / macOS

```bash
# Generate build tree & Compile
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Launch Executable
cd bin
./BankSystem
```

---

## 📂 Repository Structure

```text
NexusBank-Core-OOP-Engine/
├── CMakeLists.txt                      # Root Build Configuration
├── README.md                           # Enterprise Architectural Documentation
│
├── bin/                                # Compiled Executables & Output Target
│   └── BankSystem                      # Executable Output
│
├── data/                               # Persistent Storage Flat-Files
│   ├── Clients.txt                     # Bank Account Ledgers
│   ├── Users.txt                       # User Accounts & Bitmasks
│   ├── Currencies.txt                  # International Exchange Rates
│   ├── LoginRegister.txt               # Authentication Audit Logs
│   └── TransferLog.txt                 # Transfer Transaction History
│
├── include/                            # Header Architecture
│   ├── Global.h                        # Session State & Adaptive Path Resolvers
│   │
│   ├── Core/                           # Business Domain Models
│   │   ├── clsPerson.h                 # Abstract Person Entity
│   │   ├── clsBankClient.h             # Financial Account Entity
│   │   ├── clsUser.h                   # IAM & RBAC Entity
│   │   └── clsCurrency.h               # FX Entity & Arbitrage Calculator
│   │
│   ├── Lib/                            # Foundation Toolkits
│   │   ├── DateLib.h                   # DateTime Calculations & Calendar Module
│   │   ├── MyStringLib.h               # String Processing & Tokenizer Engine
│   │   ├── InputValidateLib.h          # Input Sanitization & Masked Password Engine
│   │   └── UtilLib.h                   # Ciphers, ASCII Color Formatter & UI Effects
│   │
│   └── UI/                             # Presentation Views & Navigation
│       ├── Common/clsScreen.h          # UI Base Screen & Session Status Bar
│       ├── Main/clsMainScreen.h        # Root Application Dashboard
│       ├── Auth/                       # Security & User Management Views
│       ├── Clients/                    # Account Management Views
│       ├── Transactions/               # Financial Operations & Balance Views
│       └── FX/                         # Foreign Exchange & Rate Config Views
│
└── src/                                # Core Implementations (.cpp)
    ├── Main.cpp                        # Entry Point
    ├── DateLib.cpp                     # DateTime Computation Routines
    ├── MyStringLib.cpp                 # String Helper Routines
    ├── InputValidateLib.cpp            # Validation Routines
    └── UtilLib.cpp                     # Utility, Cryptographic & Color Routines
```

---

## 🔑 Default Credentials & Initial State

| Attribute | Default Value | Notes |
| :--- | :--- | :--- |
| **Username** | `Admin` | Case-insensitive in lookups |
| **Password** | `1234` | Masked in UI, encrypted as `;<=>` on disk |
| **Permissions** | Full Access (`-1`) | Bitwise bypass for all menus & operations |

---

## 📬 Contact & Developer Info

<p align="left">
  <a href="https://github.com/Abdullah-Shalgam"><img src="https://img.shields.io/badge/GitHub-Abdullah--Shalgam-181717?style=for-the-badge&logo=github" alt="GitHub"></a>
  <a href="https://instagram.com/abdullah_shalgam"><img src="https://img.shields.io/badge/Instagram-@abdullah__shalgam-E4405F?style=for-the-badge&logo=instagram" alt="Instagram"></a>
  <a href="https://wa.me/218931364346"><img src="https://img.shields.io/badge/WhatsApp-%2B218%2093%20136%204346-25D366?style=for-the-badge&logo=whatsapp" alt="WhatsApp"></a>
  <a href="mailto:bdallhshlghwm500@gmail.com"><img src="https://img.shields.io/badge/Email-bdallhshlghwm500%40gmail.com-D14836?style=for-the-badge&logo=gmail&logoColor=white" alt="Email"></a>
</p>
