# Arctic Research Station Management System

A C++ resource management system for a simulated Arctic research station, built as part of the **INFO1102 - Principles of Programming II** course.

## 📋 Project Overview

This program manages critical resources (food, fuel, oxygen, etc.) at an isolated Arctic research station. It tracks inventory, logs all operations, and ensures robust error handling — because in the Arctic, a software crash could be fatal!

## ⚙️ Features

- **Load Data** – Reads resources from `database1.txt` into a structured array at startup
- **Display Resources** – Shows the current state of all resources (name, quantity, type, spec)
- **Consume a Resource** – Updates stock after consumption, with error handling for insufficient quantities
- **Sort by Quantity** – Sorts resources in ascending order using a Bubble Sort algorithm
- **Operation Logging** – Every action is saved to `journal_bord.txt` using `ios::app`

## 🗂️ Project Structure
```
├── bearivo_jovial.cpp     # Main source file
├── database1.txt          # Input data file (resources)
└── journal_bord.txt       # Auto-generated operation log
```

## 🧠 Key Concepts Used

- **Enum** – `TypeRessource` (VITAL, TECHNIQUE, CONSOMMABLE)
- **Union** – `SpecInfo` stores either a pressure (`int`) or temperature (`double`)
- **Struct** – `RessourceJB` combines all resource attributes
- **Pointers** – Functions use pointers to directly modify data in memory (no copy overhead)
- **File I/O** – `ifstream` for reading, `ofstream` with `ios::app` for logging
- **Exception Handling** – `try/catch` blocks for file errors, invalid input, and stock overflow

## 🔢 Sorting Algorithm

Implements **Bubble Sort** — O(n²) time complexity. Simple and sufficient for a small fixed array of 5 resources.

## 🚀 How to Run
```bash
# Compile
g++ bearivo_jovial.cpp -o station

# Run
./station
```

Make sure `database1.txt` is in the same directory as the executable.

## 📄 License

MIT License — Copyright (c) 2026 Jovial Elyas Bearivo
