#include "Trie.h"
#include <algorithm>
#include <cctype>

// ─── Helpers ──────────────────────────────────────────────────────────────────

std::string Trie::normalize(const std::string& word) {
    std::string result;
    for (char c : word) {
        if (std::isalpha(static_cast<unsigned char>(c))) {
            result += std::tolower(static_cast<unsigned char>(c));
        }
    }
    return result;
}

bool Trie::hasChildren(TrieNode* node) {
    for (int i = 0; i < 26; i++) {
        if (node->children[i] != nullptr) return true;
    }
    return false;
}

// ─── Constructor / Destructor ─────────────────────────────────────────────────

Trie::Trie() {
    root = new TrieNode();
}

Trie::~Trie() {
    delete root;
}

// ─── DSA1: insert ─────────────────────────────────────────────────────────────

void Trie::insert(const std::string& word, const std::string& code, const std::string& def) {
    std::string key = normalize(word);
    if (key.empty()) return;

    TrieNode* current = root;
    for (char c : key) {
        int index = c - 'a';
        if (current->children[index] == nullptr) {
            current->children[index] = new TrieNode();
        }
        current = current->children[index];
    }
    current->isEndOfWord  = true;
    current->billingCode  = code;
    current->definition   = def;
}

// ─── DSA2: searchExact ────────────────────────────────────────────────────────

TrieNode* Trie::searchExact(const std::string& word) {
    std::string key = normalize(word);
    TrieNode* current = root;
    for (char c : key) {
        int index = c - 'a';
        if (current->children[index] == nullptr) {
            return nullptr;   // path doesn't exist
        }
        current = current->children[index];
    }
    if (current->isEndOfWord) return current;
    return nullptr;           // prefix exists but not a complete word
}

// ─── DSA3: getSuggestions ─────────────────────────────────────────────────────

std::vector<std::string> Trie::getSuggestions(const std::string& prefix) {
    std::string key = normalize(prefix);
    TrieNode* current = root;

    for (char c : key) {
        int index = c - 'a';
        if (current->children[index] == nullptr) {
            return {};   // no words match this prefix
        }
        current = current->children[index];
    }

    std::vector<std::string> results;
    collectAll(current, key, results);
    std::sort(results.begin(), results.end());
    return results;
}

// ─── DSA4: collectAll (DFS helper) ───────────────────────────────────────────

void Trie::collectAll(TrieNode* node, const std::string& prefix, std::vector<std::string>& results) {
    if (node->isEndOfWord) {
        results.push_back(prefix);
    }
    for (int i = 0; i < 26; i++) {
        if (node->children[i] != nullptr) {
            char nextChar = static_cast<char>('a' + i);
            collectAll(node->children[i], prefix + nextChar, results);
        }
    }
}

// ─── DSA5: deleteWord ─────────────────────────────────────────────────────────

bool Trie::deleteWord(const std::string& word) {
    std::string key = normalize(word);
    return deleteHelper(root, key, 0);
}

bool Trie::deleteHelper(TrieNode* node, const std::string& word, int depth) {
    if (node == nullptr) return false;

    if (depth == static_cast<int>(word.length())) {
        if (!node->isEndOfWord) return false;   // word doesn't exist
        node->isEndOfWord  = false;
        node->billingCode  = "";
        node->definition   = "";
        return !hasChildren(node);              // safe to prune if no children
    }

    int index = word[depth] - 'a';
    if (deleteHelper(node->children[index], word, depth + 1)) {
        delete node->children[index];
        node->children[index] = nullptr;
        return (!node->isEndOfWord) && (!hasChildren(node));
    }
    return false;
}

// ─── DSA6: updateTerm ────────────────────────────────────────────────────────

bool Trie::updateTerm(const std::string& word, const std::string& code, const std::string& def) {
    TrieNode* node = searchExact(word);
    if (node == nullptr) return false;
    node->billingCode = code;
    node->definition  = def;
    return true;
}

// ─── Utility: collectAllWords (for CSV persistence) ──────────────────────────

void Trie::collectAllWords(std::vector<std::tuple<std::string, std::string, std::string>>& out) {
    collectAllHelper(root, "", out);
}

void Trie::collectAllHelper(TrieNode* node, const std::string& prefix,
                            std::vector<std::tuple<std::string, std::string, std::string>>& out) {
    if (node->isEndOfWord) {
        out.emplace_back(prefix, node->billingCode, node->definition);
    }
    for (int i = 0; i < 26; i++) {
        if (node->children[i] != nullptr) {
            collectAllHelper(node->children[i], prefix + static_cast<char>('a' + i), out);
        }
    }
}
