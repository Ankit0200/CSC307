#pragma once
#include "Trie.h"
#include <string>

class CLI {
public:
    CLI(Trie& trie, const std::string& dataFile);
    void run();

private:
    Trie&       trie;
    std::string dataFile;

    void printBanner();
    void printMenu();

    void handleAutoComplete();
    void handleExactSearch();
    void handleInsert();
    void handleUpdate();
    void handleDelete();
    void handleSave();

    std::string promptLine(const std::string& label);
    bool        confirmAction(const std::string& msg);
};
