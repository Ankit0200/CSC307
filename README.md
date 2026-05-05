# MedTrie Auto-Completer

**Authors:** Ankit Devkota, Manish Katel, Nidhish Acharya
**Course:** CSC 307
**Semester:** Spring 2026

---

## 1. Prerequisites & Environment

Before running this project, ensure your system meets the following requirements:

* **Operating System:** Linux (Ubuntu 20.04+), macOS, or Windows (with WSL or MinGW)
* **Compiler:** GCC 9.0+ or Clang 10+ with C++17 support
* **Build System:** GNU Make

### Required Libraries

This project uses only the C++17 Standard Library — no external dependencies are required.

* **`<fstream>` / `<sstream>`**: CSV file I/O for dictionary persistence
* **`<vector>` / `<tuple>` / `<algorithm>`**: Data collection and sorting of Trie entries
* **`<cctype>`**: Character normalization (lowercase conversion, alpha filtering)

---

## 2. Installation & Setup

1. Ensure `g++` (GCC 9+) and `make` are installed:
   ```bash
   # Ubuntu / Debian
   sudo apt update && sudo apt install g++ make

   # macOS (via Homebrew)
   brew install gcc make
   ```

2. Place the following source files in the same directory:
   ```
   main.cpp
   Trie.cpp       Trie.h
   TrieNode.cpp   TrieNode.h
   Storage.cpp    Storage.h
   CLI.cpp        CLI.h
   Makefile
   dictionary.csv   ← optional; program starts empty if absent
   ```

3. (Optional) Clone from version control if hosted:
   ```bash
   git clone <repository-url>
   cd medtrie
   ```

---

## 3. Build Instructions

**Using Make (recommended):**

```bash
# Compile the project
make

# Compile and run immediately with the default dictionary
make run

# Remove the compiled binary
make clean
```

**Using the compiler directly:**

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -O2 \
    -o medtrie main.cpp TrieNode.cpp Trie.cpp Storage.cpp CLI.cpp
```

The output binary will be named `medtrie`.

---

## 4. Execution Guide

* **Executable Name:** `medtrie`
* **Default run** (loads `dictionary.csv` from the current directory):
  ```bash
  ./medtrie
  ```
* **Custom dictionary file** (pass path as the first argument):
  ```bash
  ./medtrie /path/to/my_dictionary.csv
  ```
* **Assets:** The executable must be run from the directory that contains `dictionary.csv` (or whichever CSV you specify) so the program can load and save the dictionary correctly.

### Menu Options

Once launched, the interactive CLI presents the following options:

| Option | Feature |
|--------|---------|
| `1` | Auto-complete — retrieve all terms matching a given prefix |
| `2` | Exact term lookup — view ICD-10 code and definition |
| `3` | Insert new term — add a medical term, billing code, and definition |
| `4` | Update existing term — modify the billing code or definition |
| `5` | Delete a term — remove a term from the dictionary |
| `6` | Save dictionary to file — persist current state to CSV |
| `0` | Exit — optionally save before quitting |

---

## 5. Troubleshooting / Known Issues

* **`command not found: make`** — Install `make` via your package manager (`apt`, `brew`, etc.) as shown in Section 2.
* **`g++: error: unrecognized command-line option '-std=c++17'`** — Your GCC version is too old. Upgrade to GCC 9 or later.
* **Dictionary not loading** — Ensure `dictionary.csv` is in the same directory as the executable, or pass the correct path as a command-line argument. The file must follow the format `term,billing_code,definition` with an optional header row.
* **ANSI color codes not displaying** — If running on a terminal that does not support ANSI escape codes (e.g., some Windows terminals without WSL), color formatting will appear as raw escape sequences. Use Windows Terminal, WSL, or a compatible terminal emulator.
* **Non-alpha characters stripped from terms** — The Trie normalizes all input to lowercase and removes non-alphabetic characters (spaces, hyphens, digits). Searching for `"type-2"` and `"type2"` will yield the same result, and terms are stored without those characters.