/**
 * File that defines an HCNode that will be placed in an HCTree.
 *
 * Author: James Chong
 * Email: j2chong@ucsd.edu
 */
#ifndef HCNODE_HPP
#define HCNODE_HPP

#include <iostream>

typedef unsigned char byte;

using namespace std;

/** A class, instances of which are nodes in an HCTree.
 */
class HCNode {
  public:
    unsigned int count;  // the freqency of the symbol
    byte symbol;         // byte in the file we're keeping track of
    HCNode* c0;          // pointer to '0' child
    HCNode* c1;          // pointer to '1' child
    HCNode* p;           // pointer to parent

    /* Constructor that initialize a HCNode */
    HCNode(unsigned int count, byte symbol, HCNode* c0 = 0, HCNode* c1 = 0,
           HCNode* p = 0)
        : count(count), symbol(symbol), c0(c0), c1(c1), p(p) {}
};

/* For printing an HCNode to an ostream. Possibly useful for debugging */
ostream& operator<<(ostream&, const HCNode&) __attribute__((weak));
ostream& operator<<(ostream& stm, const HCNode& n) {
    stm << "[" << n.count << "," << (int)(n.symbol) << "]";
    return stm;
}

/* Comparator of HCNode pointer. In priority queue, HCNode ptr with lower count
 * has higher priority, and if count is the same, HCNode ptr with larger symbol
 * has higher prioruty.
 */
struct HCNodePtrComp {
    /* Comparator of HCNode that return the node with the lower count,
     * or the larger ascii value if count equal
     * lhs: first node being compared
     * rhs: second node being compared
     */
    bool operator()(HCNode*& lhs, HCNode*& rhs) const {
        if (lhs->count != rhs->count) {
            return lhs->count > rhs->count;
        }
        return lhs->symbol > rhs->symbol;
    }
};
#endif  // HCNODE_HPP
