#include "Storage.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <tuple>
#include <vector>

// ─── CSV Helpers ──────────────────────────────────────────────────────────────

// Escape a field for CSV: wrap in quotes, double any internal quotes.
static std::string csvEscape(const std::string& field) {
    bool needsQuote = field.find_first_of(",\"\n\r") != std::string::npos;
    if (!needsQuote) return field;
    std::string out = "\"";
    for (char c : field) {
        if (c == '"') out += '"';
        out += c;
    }
    out += '"';
    return out;
}

// Parse one CSV line into fields (handles quoted fields with embedded commas).
static std::vector<std::string> parseCSVLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool inQuotes = false;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (inQuotes) {
            if (c == '"') {
                if (i + 1 < line.size() && line[i + 1] == '"') {
                    field += '"';
                    ++i;
                } else {
                    inQuotes = false;
                }
            } else {
                field += c;
            }
        } else {
            if (c == '"') {
                inQuotes = true;
            } else if (c == ',') {
                fields.push_back(field);
                field.clear();
            } else {
                field += c;
            }
        }
    }
    fields.push_back(field);
    return fields;
}

// ─── Load ─────────────────────────────────────────────────────────────────────

int Storage::loadFromCSV(Trie& trie, const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        // File not found – start with empty dictionary
        return 0;
    }

    int count = 0;
    std::string line;

    // Skip optional header row
    if (std::getline(file, line)) {
        // If the first field is "term" (case-insensitive header), skip it
        auto fields = parseCSVLine(line);
        if (!fields.empty() && (fields[0] == "term" || fields[0] == "Term")) {
            // It's a header – continue to data rows
        } else {
            // It's data – insert it
            if (fields.size() >= 3) {
                trie.insert(fields[0], fields[1], fields[2]);
                ++count;
            }
        }
    }

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        auto fields = parseCSVLine(line);
        if (fields.size() >= 3) {
            trie.insert(fields[0], fields[1], fields[2]);
            ++count;
        }
    }

    return count;
}

// ─── Save ─────────────────────────────────────────────────────────────────────

int Storage::saveToCSV(Trie& trie, const std::string& filepath) {
    std::vector<std::tuple<std::string, std::string, std::string>> words;
    trie.collectAllWords(words);

    std::ofstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: could not open " << filepath << " for writing.\n";
        return -1;
    }

    file << "term,billing_code,definition\n";
    for (auto& [term, code, def] : words) {
        file << csvEscape(term) << ','
             << csvEscape(code) << ','
             << csvEscape(def)  << '\n';
    }

    return static_cast<int>(words.size());
}
