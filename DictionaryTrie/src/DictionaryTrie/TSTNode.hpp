/**
 * File that creates individual nodes within a Ternary Search Trie
 *
 * Author: James Chong
 * Email: j2chong@ucsd.edu
 */
#ifndef TSTNODE_HPP
#define TSTNODE_HPP
#include <iomanip>
#include <iostream>
using namespace std;

/* The class to initialize a single node within the TST */
class TSTNode {
  public:
    // Pointers to a node's left, right, and middle nodes
    TSTNode* left;
    TSTNode* right;
    TSTNode* middle;

    // Letter to be inserted into the node
    char const data;

    // Marks if node is the end of the word
    bool isEndWord;

    // Marks if node has been visited
    bool visited;

    // Frequency word appears
    int freq;

    /* Initializes a Ternary Search Trie Node */
    TSTNode(const char& d) : data(d) {
        left = nullptr;
        right = nullptr;
        middle = nullptr;
        isEndWord = false;
        freq = 0;
    }
};

#endif  // TSTNODE_HPP