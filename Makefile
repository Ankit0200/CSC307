CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2
TARGET   = medtrie
SRCS     = main.cpp TrieNode.cpp Trie.cpp Storage.cpp CLI.cpp

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

run: all
	./$(TARGET) dictionary.csv

clean:
	rm -f $(TARGET)
