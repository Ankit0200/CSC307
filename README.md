# MedTrie

MedTrie is a C++ command-line application for storing and searching medical terminology with ICD-10 billing codes and definitions. It uses a trie data structure to support fast prefix-based auto-complete, exact term lookup, insertion, updates, deletion, and CSV persistence.

## Features

- Prefix auto-complete for medical terms
- Exact medical term lookup
- ICD-10 billing code and definition display
- Insert new terms during a CLI session
- Update existing billing codes and definitions
- Delete terms with confirmation
- Save and load dictionary data from CSV

## Project Structure

```text
.
|-- CLI.cpp / CLI.h           # Interactive command-line menu and handlers
|-- Trie.cpp / Trie.h         # Trie data structure and dictionary operations
|-- TrieNode.cpp / TrieNode.h # Trie node storage
|-- Storage.cpp / Storage.h   # CSV loading and saving
|-- main.cpp                  # Application entry point
|-- dictionary.csv            # Default medical dictionary data
|-- Makefile                  # Build and run commands
`-- medtrie                   # Compiled executable, if already built
```

## Requirements

- A C++17-compatible compiler, such as `g++`
- `make`, if using the included Makefile

## Build

From the project directory:

```bash
make
```

This builds the executable named `medtrie`.

To remove the compiled executable:

```bash
make clean
```

## Run

Run with the default dictionary file:

```bash
make run
```

Or run the executable directly:

```bash
./medtrie dictionary.csv
```

If no CSV path is provided, the program uses `dictionary.csv` by default:

```bash
./medtrie
```

You can also provide a custom CSV file:

```bash
./medtrie path/to/your_dictionary.csv
```

## CSV Format

The dictionary file should use this header:

```csv
term,billing_code,definition
```

Example:

```csv
term,billing_code,definition
appendicitis,K37,Inflammation of the appendix
asthma,J45.909,Chronic inflammatory disease of the airways
```

Fields containing commas, quotes, or newlines are escaped when saved by the application.

## CLI Menu

When the program starts, it loads terms from the selected CSV file and opens an interactive menu:

```text
1. Auto-complete (prefix search)
2. Exact term lookup
3. Insert new term
4. Update existing term
5. Delete a term
6. Save dictionary to file
0. Exit
```

Choose an option by entering its number. On exit, the program asks whether to save changes back to the CSV file.

## Notes

- Term matching is case-insensitive.
- Trie indexing normalizes terms to lowercase alphabetic characters.
- Billing codes and definitions are stored at the terminal trie node for each term.
- Auto-complete results are returned in alphabetical order.
