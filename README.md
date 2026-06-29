# 📦 Inventory Management System (C++)

A feature-rich, command-line Inventory Management System built in **C++** using object-oriented programming. Manage products, track stock, simulate sales, and persist data — all from your terminal.

---

## 📋 Table of Contents

- [Features](#features)
- [Project Structure](#project-structure)
- [Prerequisites](#prerequisites)
- [Build Instructions](#build-instructions)
- [Running the Program](#running-the-program)
- [Menu Options](#menu-options)
- [Data Persistence](#data-persistence)
- [Design Details](#design-details)

---

## ✨ Features

### Core Features
| # | Feature | Description |
|---|---|---|
| 1 | **Add New Product** | Add a product with unique ID, name, category, price & quantity |
| 2 | **Update Stock** | Restock, deduct, or set absolute stock level |
| 3 | **Sell Product** | Reduce stock and record the sale in transaction history |
| 4 | **Search Product** | Search by ID or partial product name (case-insensitive) |
| 5 | **Display All Products** | View all products with 6 sorting options |
| 6 | **Low Stock Alert** | Highlight items at or below 5 units |
| 7 | **Delete Product** | Remove a product with a confirmation prompt |
| 8 | **Save to File** | Persist all data to `inventory.dat` and `transactions.dat` |
| 9 | **Load from File** | Restore inventory from saved files (auto-loads on startup) |

### Intermediate / Advanced Features
| # | Feature | Description |
|---|---|---|
| 10 | **Generate Demo Products** | Randomly populate inventory for testing |
| 11 | **Total Inventory Value** | Calculate the grand total value of all stock (`price × qty`) |
| 12 | **Restock Suggestions** | Lists under-stocked items with suggested order qty & estimated cost |
| 13 | **Transaction History** | View last 10 sales with timestamps and revenue totals |

---

## 🗂️ Project Structure

```
InventoryManagementSystem/
│
├── main.cpp            # Main menu loop & program entry point
├── Inventory.h         # Inventory class declaration
├── Inventory.cpp       # All business logic, file I/O, sorting, simulation
├── Product.h           # Product and Transaction struct definitions
├── Console.h           # Console color utilities & validated input helpers
│
├── inventory.exe       # Compiled binary (generated after build)
├── inventory.dat       # Auto-generated product database (plain text)
└── transactions.dat    # Auto-generated transaction history (plain text)
```

---

## 🔧 Prerequisites

- **OS**: Windows (tested), Linux/macOS (supported via ANSI fallback)
- **Compiler**: `g++` with C++11 support
  - Windows: [MinGW-w64](https://www.mingw-w64.org/) or [TDM-GCC](https://jmeubank.github.io/tdm-gcc/)
  - Linux/macOS: GCC via `sudo apt install g++` or `brew install gcc`

### Verify your compiler installation:
```bash
g++ --version
```
Expected output (example):
```
g++.exe (MinGW.org GCC-6.3.0-1) 6.3.0
```

---

## 🔨 Build Instructions

Navigate to the project directory and run:

```bash
g++ -std=c++11 main.cpp Inventory.cpp -o inventory.exe
```

| Flag | Purpose |
|---|---|
| `-std=c++11` | Enable C++11 standard (required for lambdas, `<chrono>`, etc.) |
| `main.cpp Inventory.cpp` | Source files to compile |
| `-o inventory.exe` | Name the output executable |

---

## ▶️ Running the Program

```bash
.\inventory.exe         # Windows (PowerShell / CMD)
./inventory.exe         # Linux / macOS
```

> **Note:** The program automatically loads `inventory.dat` and `transactions.dat` on startup if they exist.

---

## 📟 Menu Options

```
======================================================================
               INVENTORY MANAGEMENT SYSTEM (C++)
======================================================================
 1.  Add New Product
 2.  Update Stock Level
 3.  Sell Product (Deduct Stock)
 4.  Search Product (ID/Name)
 5.  Display All Products
 6.  Show Low Stock Items
 7.  Calculate Total Inventory Value
 8.  Generate Random Products (Demo)
 9.  Save to File
 10. Load from File
---------------------- Intermediate Features -------------------------
 11. Delete Product
 12. View Restock Suggestions
 13. View Transaction History (Last 10 Sales)
----------------------------------------------------------------------
 0.  Exit System
----------------------------------------------------------------------
```

### Quick Start (Demo)
1. Run the program — `.\inventory.exe`
2. Press **8** → Generate random demo products
3. Press **5** → Display and sort all products
4. Press **3** → Sell a product to generate a transaction
5. Press **13** → View transaction history
6. Press **12** → View restock suggestions
7. Press **9** → Save data to file
8. Press **0** → Exit, then re-run to confirm data persists

---

## 💾 Data Persistence

All data is stored in plain-text files in the **same directory as the executable**:

| File | Content |
|---|---|
| `inventory.dat` | Product records (ID, name, category, price, quantity) |
| `transactions.dat` | Last 10 sale records (timestamp, product, qty, amount) |

- Data is **saved** via option `9` (Save to File)
- Data is **loaded** via option `10` (Load from File) **and automatically on startup**
- Files use a structured newline-delimited text format — no binary encoding

---

## 🎨 Design Details

- **OOP Architecture** — Business logic is encapsulated in the `Inventory` class; UI concerns are separated into `Console.h`
- **Windows Console Colors** — Uses `SetConsoleTextAttribute` (Windows API) with an ANSI escape code fallback for Linux/macOS
- **Robust Input Validation** — All numeric inputs are validated with type-checking and range enforcement; invalid inputs prompt a retry
- **Sorting** — 6 sort modes for the product list: by ID, Name, Price (asc/desc), Quantity, Category
- **Low Stock Threshold** — Hardcoded at ≤ 5 units; items below this are highlighted in red in all views
- **Restock Target** — Suggestions aim to bring stock up to 25 units with per-item estimated cost

---

## 👨‍💻 Author

**Huzaifa** — C++ Inventory Management System Project
