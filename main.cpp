#include "Trie.h"
#include "Storage.h"
#include "CLI.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string dataFile = "dictionary.csv";

    // Allow an optional data-file path as a CLI argument
    if (argc >= 2) {
        dataFile = argv[1];
    }

    Trie trie;

    // Load persisted dictionary
    int loaded = Storage::loadFromCSV(trie, dataFile);
    if (loaded > 0) {
        std::cout << "\033[32mLoaded " << loaded
                  << " terms from " << dataFile << ".\033[0m\n";
    } else {
        std::cout << "\033[33mNo existing dictionary found at " << dataFile
                  << ". Starting with an empty dictionary.\033[0m\n";
    }

    // Launch CLI
    CLI cli(trie, dataFile);
    cli.run();

    return 0;
}
