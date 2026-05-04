#include "TrieNode.h"
#include <cstring>

TrieNode::TrieNode() : isEndOfWord(false) {
    for (int i = 0; i < 26; i++) {
        children[i] = nullptr;
    }
}

TrieNode::~TrieNode() {
    for (int i = 0; i < 26; i++) {
        delete children[i];
        children[i] = nullptr;
    }
}
