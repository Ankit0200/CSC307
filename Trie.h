#pragma once
#include "TrieNode.h"
#include <string>
#include <vector>

class Trie {
public:
    Trie();
    ~Trie();

    // Insert a medical term with its billing code and definition
    void insert(const std::string& word, const std::string& code, const std::string& def);

    // Search for an exact term; returns pointer to end node or nullptr
    TrieNode* searchExact(const std::string& word);

    // Returns all terms beginning with the given prefix
    std::vector<std::string> getSuggestions(const std::string& prefix);

    // Delete a term from the Trie; returns true if found and deleted
    bool deleteWord(const std::string& word);

    // Update billing code and definition for an existing term
    bool updateTerm(const std::string& word, const std::string& code, const std::string& def);

    // Collect all words in the Trie (for CSV save)
    void collectAllWords(std::vector<std::tuple<std::string, std::string, std::string>>& out);

private:
    TrieNode* root;

    // DFS helper to collect all completions from a given node
    void collectAll(TrieNode* node, const std::string& prefix, std::vector<std::string>& results);

    // Recursive delete helper; returns true if the node can be pruned
    bool deleteHelper(TrieNode* node, const std::string& word, int depth);

    // Helper to check if a node has any children
    bool hasChildren(TrieNode* node);

    // Helper to collect all (word, code, def) triples
    void collectAllHelper(TrieNode* node, const std::string& prefix,
                          std::vector<std::tuple<std::string, std::string, std::string>>& out);

    // Normalizes input to lowercase, strips non-alpha characters for Trie indexing
    std::string normalize(const std::string& word);
};
