/**
 * Implements methods to build HCTree, encode, and decode given parameters
 *
 * Author: James Chong
 * Email: j2chong@ucsd.edu
 */
#include "HCTree.hpp"
#include <bits/stdc++.h>
#include <queue>

#define EMPTY_STRING ""
#define ZERO_CHAR '0'
#define ONE_CHAR '1'
#define NEWLINE_CHAR '\n'

using namespace std;

/* Deletes all nodes within HCTree */
HCTree::~HCTree() { deleteAll(root); }

/* Creates the HCTree from given freqs vector using a priority queue
 * freqs: vector mapping frequency values to each of its ascii indicies
 */
void HCTree::build(const vector<unsigned int>& freqs) {
    // Creates priority queue giving priority to larger frequencies, then larger
    // ascii values if frequency is tied
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

    // Loop through each index in freqs
    for (int i = 0; i < freqs.size(); i++) {
        // If freq a non-zero value, create new node, push into queue, and add
        // to leaves vector
        if (freqs[i] != 0) {
            HCNode* p = new HCNode(freqs[i], i, nullptr, nullptr, nullptr);
            pq.push(p);
            leaves.push_back(p);
        } else {
            // Insert a nullptr to the leaves vector if freqs' frequency value
            // is 0
            leaves.push_back(nullptr);
        }
    }

    // Loop until priority queue is empty
    while (pq.size() > 1) {
        // Highest priority node
        HCNode* n1 = pq.top();
        pq.pop();

        // Second highest priority node
        HCNode* n2 = pq.top();
        pq.pop();

        // Create parent node of n1 and n2, summing their counts, and taking the
        // symbol of the higher priority node
        HCNode* parentNode =
            new HCNode(n1->count + n2->count, n1->symbol, n1, n2, nullptr);
        n1->p = parentNode;
        n2->p = parentNode;

        // Push parent node into queue
        pq.push(parentNode);
    }

    // Set root to the last node in the priority queue
    root = pq.top();
}

/* Encode the symbol passed in using bitwise I/O
 * symbol: character to be encoded
 * out: BitOutputStream symbol is to be written to
 */
void HCTree::encode(byte symbol, BitOutputStream& out) const {
    // Find the leaf of the symbol
    HCNode* curr = leaves[symbol];

    stack<int> s;

    // Loop until root is reached
    while (curr->p != nullptr) {
        // Push 0 to the stack if current node is on the left of its parent
        if (curr->p->c0 == curr) {
            s.push(0);
            // Push 1 to the stack is current node is on the right of its parent
        } else {
            s.push(1);
        }

        // Move to parent
        curr = curr->p;
    }

    // Loop until stack is empty
    while (s.empty() != true) {
        // Write bit at top of the stack to out, then remove
        int bitVal = s.top();
        out.writeBit(bitVal);
        s.pop();
    }
}

/* Enocde given symbol using 0's and 1's as characters
 * symbol: character to be encoded
 * out: ostream to be written to
 */
void HCTree::encode(byte symbol, ostream& out) const {
    // Find leaf with symbol
    HCNode* curr = leaves[symbol];
    string encoded = EMPTY_STRING;

    // Loop until root reached
    while (curr->p != nullptr) {
        // Add a 0 to the beginning of the string if node is on the left of
        // parent
        if (curr->p->c0 == curr) {
            encoded = ZERO_CHAR + encoded;
            // Add a 1 to the beginning of the string if node is on the right of
            // parent
        } else {
            encoded = ONE_CHAR + encoded;
        }

        // Move to parent
        curr = curr->p;
    }

    // Send each character of the string encoded to out
    for (int i = 0; i < encoded.size(); i++) {
        out << encoded[i];
    }
}

/* Decode sequence of bits using bitwise I/O
 * in: BitInputStream to decode from
 */
byte HCTree::decode(BitInputStream& in) const {
    // Set current node to root
    HCNode* curr = root;

    // Loop until leaf is reached
    while (curr->c0 != nullptr && curr->c1 != nullptr) {
        // Read the current bit
        int bit = in.readBit();

        // Traverse left if bit is 0
        if (bit == 0) {
            curr = curr->c0;
        }

        // Traverse right if bit is 0
        if (bit == 1) {
            curr = curr->c1;
        }
    }

    return curr->symbol;  // Return decoded character
}

/* Decode sequence of bits from in
 * in: istream to decode from
 */
byte HCTree::decode(istream& in) const {
    // Set current node as root
    HCNode* curr = root;

    int nextByte;
    unsigned char nextChar;

    // Loop until newline char or EOF is reached
    while ((nextByte = in.get()) != NEWLINE_CHAR && nextByte != EOF) {
        // Get next byte from istream and convert to unsigned char
        nextChar = (unsigned char)nextByte;

        // If char is 0, traverse left
        if (nextChar == ZERO_CHAR) {
            curr = curr->c0;
        }

        // If char is 1, traverse right
        if (nextChar == ONE_CHAR) {
            curr = curr->c1;
        }
    }

    return curr->symbol;  // Return decoded character
}

/* Helper method to recursively delete nodes in trie
 *  node: node to be deleted
 */
void HCTree::deleteAll(HCNode* node) {
    // If tree empty, return
    if (node == nullptr) {
        return;
    }

    // Recurse left to delete
    deleteAll(node->c0);

    // Recurse right to delete
    deleteAll(node->c1);

    // Delete current node
    delete node;
}
