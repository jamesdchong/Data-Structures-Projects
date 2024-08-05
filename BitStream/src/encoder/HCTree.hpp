/**
 * Header file that defines all public and private variables and methods to be
 * implemented in HCTree.cpp
 *
 * Author: James Chong
 * Email: j2chong@ucsd.edu
 */
#ifndef HCTREE_HPP
#define HCTREE_HPP

#include <fstream>
#include <queue>
#include <vector>
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"
#include "HCNode.hpp"

using namespace std;

/* Class that defines HCTree containing a root and a leaves vector pointing to
 * its leaves
 */
class HCTree {
  private:
    HCNode* root;            // the root of HCTree
    vector<HCNode*> leaves;  // a vector storing pointers to all leaf HCNodes

    /* Helper method to recursively delete nodes in trie
     *  node: node to be deleted
     */
    void deleteAll(HCNode* node);

  public:
    /* Constructor that initializes an empty HCTree */
    HCTree() { root = nullptr; }

    /* Deletes all nodes within HCTree */
    ~HCTree();

    /* Creates the HCTree from given freqs vector using a priority queue
       freqs: vector mapping frequency values to each of its ascii indicies
     */
    void build(const vector<unsigned int>& freqs);

    /* Encode the symbol passed in using bitwise I/O
     * symbol: character to be encoded
     * out: BitOutputStream symbol is to be written to
     */
    void encode(byte symbol, BitOutputStream& out) const;

    /* Enocde given symbol using 0's and 1's as characters
     * symbol: character to be encoded
     * out: ostream to be written to
     */
    void encode(byte symbol, ostream& out) const;

    /* Decode sequence of bits using bitwise I/O
     * in: BitInputStream to decode from
     */
    byte decode(BitInputStream& in) const;

    /* Decode sequence of bits from in
     * in: istream to decode from
     */
    byte decode(istream& in) const;
};

#endif  // HCTREE_HPP
