#ifndef BST_HPP
#define BST_HPP
#include <iostream>
#include <vector>
#include "BSTIterator.hpp"
#include "BSTNode.hpp"
using namespace std;

template <typename Data>
class BST {
  protected:
    // pointer to the root of this BST, or 0 if the BST is empty
    BSTNode<Data>* root;

    // number of Data items stored in this BST.
    unsigned int isize;

    // height of this BST.
    int iheight;

  public:
    /** Define iterator as an aliased typename for BSTIterator<Data>. */
    typedef BSTIterator<Data> iterator;

    /** Default constructor.
     *  Initialize an empty BST.
     */
    BST() : root(0), isize(0), iheight(-1) {}

    /** TODO */
    virtual ~BST() { deleteAll(root); }

    virtual bool insert(const Data& item) {
        if (isize == 0) {
            root = new BSTNode<Data>(item);
            isize += 1;
            return true;
        }

        BSTNode<Data>* curr = root;

        while (!(!(curr->data < item) && !(item < curr->data))) {
            if (item < curr->data) {
                if (curr->left == nullptr) {
                    curr->left = new BSTNode<Data>(item);
                    curr->left->parent = curr;
                    isize += 1;
                    return true;
                } else {
                    curr = curr->left;
                }
            } else if (!(item < curr->data)) {
                if (curr->right == nullptr) {
                    curr->right = new BSTNode<Data>(item);
                    curr->right->parent = curr;
                    isize += 1;
                    return true;
                } else {
                    curr = curr->right;
                }
            }
        }
        return false;
    }

    /** TODO */
    virtual iterator find(const Data& item) const {
        if (isize == 0) {
            return BST<Data>::iterator(nullptr);
        }
        BSTNode<Data>* curr = root;

        while (!(!(curr->data < item) && !(item < curr->data))) {
            if (item < curr->data) {
                curr = curr->left;
            } else if (!(item < curr->data)) {
                curr = curr->right;
            }
            if (curr == nullptr) {
                return BST<Data>::iterator(nullptr);
            }
        }
        return BST<Data>::iterator(curr);
    }

    /** TODO */
    unsigned int size() const { return isize; }

    /** TODO */
    int height() const { return getHeight(root); }

    /** TODO */
    bool empty() const {
        if (isize <= 0) {
            return true;
        } else {
            return false;
        }
    }

    /** TODO */
    iterator begin() const { return BST::iterator(first(root)); }

    /** Return an iterator pointing past the last item in the BST.
     */
    iterator end() const { return typename BST<Data>::iterator(0); }

    /** TODO */
    vector<Data> inorder() const {
        vector<Data> vect;
        if (isize == 0) {
            return vect;
        }
        inOrder(root, vect);
        return vect;
    }

  private:
    /** TODO */
    static BSTNode<Data>* first(BSTNode<Data>* root) {
        if (root == nullptr) {
            return root;
        }
        BSTNode<Data>* curr = root;
        while (curr->left != nullptr) {
            curr = curr->left;
        }
        return curr;
    }

    /** TODO */
    static int getHeight(BSTNode<Data>* node) {
        if (node == nullptr) {
            return -1;
        } else {
            int leftCount = getHeight(node->left);
            int rightCount = getHeight(node->right);

            if (leftCount > rightCount) {
                return (leftCount + 1);
            } else {
                return (rightCount + 1);
            }
        }
    }

    /** TODO */
    static void inOrder(BSTNode<Data>* node, vector<Data>& vect) {
        if (node == nullptr) {
            return;
        }

        inOrder(node->left, vect);
        vect.push_back(node->data);
        inOrder(node->right, vect);
    }

    /** TODO */
    static void deleteAll(BSTNode<Data>* n) {
        /* Pseudocode:
           if current node is null: return;
           recursively delete left sub-tree
           recursively delete right sub-tree
           delete current node
        */
        if (n == nullptr) {
            return;
        }
        deleteAll(n->left);
        deleteAll(n->right);
        delete n;
    }
};

#endif  // BST_HPP
