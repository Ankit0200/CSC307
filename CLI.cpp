#include "CLI.h"
#include "Storage.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <limits>

// ANSI colour helpers (gracefully no-op on terminals that don't support them)
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"

CLI::CLI(Trie& trie, const std::string& dataFile)
    : trie(trie), dataFile(dataFile) {}

// ─── Prompt helpers ───────────────────────────────────────────────────────────

std::string CLI::promptLine(const std::string& label) {
    std::cout << CYAN << label << ": " << RESET;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

bool CLI::confirmAction(const std::string& msg) {
    std::cout << YELLOW << msg << " [y/N]: " << RESET;
    std::string resp;
    std::getline(std::cin, resp);
    return (!resp.empty() && (resp[0] == 'y' || resp[0] == 'Y'));
}

// ─── Banner / Menu ────────────────────────────────────────────────────────────

void CLI::printBanner() {
    std::cout << BOLD << CYAN
              << R"(
 __  __          _ _____     _
|  \/  | ___  __| |_   _| __(_) ___
| |\/| |/ _ \/ _` | | || '__| |/ _ \
| |  | |  __/ (_| | | || |  | |  __/
|_|  |_|\___|\__,_| |_||_|  |_|\___|

   Auto-Completer for Medical Terminology & ICD-10 Codes
)" << RESET << "\n";
}

void CLI::printMenu() {
    std::cout << BOLD
              << "┌─────────────────────────────────────┐\n"
              << "│           MAIN MENU                 │\n"
              << "├─────────────────────────────────────┤\n"
              << "│  1. Auto-complete (prefix search)   │\n"
              << "│  2. Exact term lookup               │\n"
              << "│  3. Insert new term                 │\n"
              << "│  4. Update existing term            │\n"
              << "│  5. Delete a term                   │\n"
              << "│  6. Save dictionary to file         │\n"
              << "│  0. Exit                            │\n"
              << "└─────────────────────────────────────┘\n"
              << RESET;
}

// ─── Feature handlers ─────────────────────────────────────────────────────────

void CLI::handleAutoComplete() {
    std::cout << "\n" << BOLD << "── Auto-Complete ──\n" << RESET;
    std::string prefix = promptLine("Enter prefix");
    if (prefix.empty()) {
        std::cout << RED << "No prefix entered.\n" << RESET;
        return;
    }

    auto suggestions = trie.getSuggestions(prefix);
    if (suggestions.empty()) {
        std::cout << YELLOW << "No terms found matching \"" << prefix << "\".\n" << RESET;
        return;
    }

    std::cout << GREEN << "\nFound " << suggestions.size()
              << " suggestion(s) for \"" << prefix << "\":\n" << RESET;

    int idx = 1;
    for (auto& s : suggestions) {
        std::cout << "  " << idx++ << ". " << s << "\n";
    }

    // Let the user drill into one of the suggestions
    std::cout << "\nEnter a number to view full details, or press Enter to go back: ";
    std::string choice;
    std::getline(std::cin, choice);
    if (choice.empty()) return;

    try {
        int n = std::stoi(choice);
        if (n >= 1 && n <= static_cast<int>(suggestions.size())) {
            TrieNode* node = trie.searchExact(suggestions[n - 1]);
            if (node) {
                std::cout << BOLD << "\nTerm       : " << RESET << suggestions[n - 1] << "\n"
                          << BOLD << "ICD-10 Code: " << RESET << node->billingCode << "\n"
                          << BOLD << "Definition : " << RESET << node->definition  << "\n\n";
            }
        }
    } catch (...) { /* invalid input – just return */ }
}

void CLI::handleExactSearch() {
    std::cout << "\n" << BOLD << "── Exact Term Lookup ──\n" << RESET;
    std::string word = promptLine("Enter medical term");
    if (word.empty()) return;

    TrieNode* node = trie.searchExact(word);
    if (!node) {
        std::cout << RED << "Term \"" << word << "\" not found in the dictionary.\n" << RESET;
        return;
    }

    std::cout << BOLD << "\nTerm       : " << RESET << word << "\n"
              << BOLD << "ICD-10 Code: " << RESET << node->billingCode << "\n"
              << BOLD << "Definition : " << RESET << node->definition  << "\n\n";
}

void CLI::handleInsert() {
    std::cout << "\n" << BOLD << "── Insert New Term ──\n" << RESET;
    std::string word = promptLine("Medical term");
    if (word.empty()) return;

    // Check for duplicate
    if (trie.searchExact(word)) {
        std::cout << YELLOW << "\"" << word << "\" already exists. Use Update to modify it.\n" << RESET;
        return;
    }

    std::string code = promptLine("ICD-10 billing code");
    std::string def  = promptLine("Definition");

    trie.insert(word, code, def);
    std::cout << GREEN << "\"" << word << "\" inserted successfully.\n" << RESET;
}

void CLI::handleUpdate() {
    std::cout << "\n" << BOLD << "── Update Existing Term ──\n" << RESET;
    std::string word = promptLine("Medical term to update");
    if (word.empty()) return;

    TrieNode* node = trie.searchExact(word);
    if (!node) {
        std::cout << RED << "\"" << word << "\" not found in the dictionary.\n" << RESET;
        return;
    }

    std::cout << BOLD << "Current ICD-10 Code: " << RESET << node->billingCode << "\n"
              << BOLD << "Current Definition : " << RESET << node->definition  << "\n";

    std::string code = promptLine("New ICD-10 billing code (Enter to keep)");
    std::string def  = promptLine("New definition (Enter to keep)");

    if (code.empty()) code = node->billingCode;
    if (def.empty())  def  = node->definition;

    trie.updateTerm(word, code, def);
    std::cout << GREEN << "\"" << word << "\" updated successfully.\n" << RESET;
}

void CLI::handleDelete() {
    std::cout << "\n" << BOLD << "── Delete Term ──\n" << RESET;
    std::string word = promptLine("Medical term to delete");
    if (word.empty()) return;

    TrieNode* node = trie.searchExact(word);
    if (!node) {
        std::cout << RED << "\"" << word << "\" not found in the dictionary.\n" << RESET;
        return;
    }

    std::cout << BOLD << "Term       : " << RESET << word << "\n"
              << BOLD << "ICD-10 Code: " << RESET << node->billingCode << "\n";

    if (confirmAction("Are you sure you want to delete this term?")) {
        trie.deleteWord(word);
        std::cout << GREEN << "\"" << word << "\" deleted.\n" << RESET;
    } else {
        std::cout << YELLOW << "Delete cancelled.\n" << RESET;
    }
}

void CLI::handleSave() {
    std::cout << "\n" << BOLD << "── Save Dictionary ──\n" << RESET;
    int written = Storage::saveToCSV(trie, dataFile);
    if (written >= 0) {
        std::cout << GREEN << "Dictionary saved (" << written
                  << " terms) to " << dataFile << ".\n" << RESET;
    }
}

// ─── Main loop ────────────────────────────────────────────────────────────────

void CLI::run() {
    printBanner();

    while (true) {
        std::cout << "\n";
        printMenu();
        std::cout << BOLD << "Choice: " << RESET;

        std::string line;
        std::getline(std::cin, line);

        if (line.empty()) continue;

        char choice = line[0];
        switch (choice) {
            case '1': handleAutoComplete(); break;
            case '2': handleExactSearch();  break;
            case '3': handleInsert();       break;
            case '4': handleUpdate();       break;
            case '5': handleDelete();       break;
            case '6': handleSave();         break;
            case '0':
                if (confirmAction("Save dictionary before exiting?")) {
                    handleSave();
                }
                std::cout << GREEN << "\nGoodbye!\n" << RESET;
                return;
            default:
                std::cout << RED << "Invalid choice. Please enter 0-6.\n" << RESET;
        }
    }
}
