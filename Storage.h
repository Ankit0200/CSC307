#pragma once
#include "Trie.h"
#include <string>

class Storage {
public:
    // Load dictionary CSV into the Trie; returns number of records loaded
    static int  loadFromCSV(Trie& trie, const std::string& filepath);

    // Save all Trie entries to CSV; returns number of records written
    static int  saveToCSV(Trie& trie, const std::string& filepath);
};
