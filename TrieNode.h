#pragma once
#include <string>

class TrieNode {
public:
    TrieNode* children[26];
    std::string billingCode;
    std::string definition;

    TrieNode();
    ~TrieNode();

private:
    bool isEndOfWord;

    // Allow Trie to access private members
    friend class Trie;
};
