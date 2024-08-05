/**
 * Creates the Dictionary Trie that contains words that are inserted along
 * with their freqencies.
 *
 * Author: James Chong
 * Email: j2chong@ucsd.edu
 */
#include "DictionaryTrie.hpp"
#include <bits/stdc++.h>
#include <iostream>
#include <stack>
#include "TSTNode.hpp"

/* Initializes an empty ternary search trie. */
DictionaryTrie::DictionaryTrie() { root = nullptr; }

/* Inserts a word into the trie and marks the last letter as the end of the word
 * along with its frequency. If an empty string is inserted or the word already
 * exists in the trie, this method returns false.
 * word: string to be inserted into the trie
 * freq: frequency this word appears
 */
bool DictionaryTrie::insert(string word, unsigned int freq) {
    // Return false if string empty
    if (word.size() == 0) {
        return false;
    }

    // Create node to traverse trie
    TSTNode* node = root;

    // Keeps track of the current letter in word
    unsigned int currIdx = 0;

    // Return false if string already exists in trie
    if (findHelper(word, root, true, freq) != nullptr) {
        return false;
    }

    // Create root node
    if (root == nullptr) {
        root = new TSTNode(word.at(currIdx));
        if (word.at(currIdx) == word.at(word.size() - 1) &&
            currIdx == word.size() - 1) {
            root->isEndWord = true;
            root->freq = freq;
            return true;
        }

        node = root;

        // Create nodes for rest of letters in word with middle
        // child
        for (auto i = 1; i < word.size(); i++) {
            node->middle = new TSTNode(word.at(i));
            node = node->middle;
        }

        // Mark last letter as end of word and add the word's
        // frequency
        node->isEndWord = true;
        node->freq = freq;
        return true;
    }

    // Loop until end of word is found
    while (true) {
        // Checks left child
        if (word.at(currIdx) < node->data) {
            // If left node exists, go left
            if (node->left != nullptr) {
                node = node->left;
            } else {
                // Create new node on left and go to it
                node->left = new TSTNode(word.at(currIdx));
                currIdx++;
                node = node->left;

                // Create nodes for rest of letters in word with the middle
                // child
                for (auto i = currIdx; i < word.size(); i++) {
                    node->middle = new TSTNode(word.at(i));
                    node = node->middle;
                    currIdx++;
                }

                // Mark last letter as end of word and add the word's frequency
                node->isEndWord = true;
                node->freq = freq;
                return true;
            }

            // Checks right child
        } else if (word.at(currIdx) > node->data) {
            // If right node exists, go right
            if (node->right != nullptr) {
                node = node->right;
            } else {
                // Create new node on right and go to it
                node->right = new TSTNode(word.at(currIdx));
                currIdx++;
                node = node->right;

                // Creates nodes for rest of letters in word with middle child
                for (auto i = currIdx; i < word.size(); i++) {
                    node->middle = new TSTNode(word.at(currIdx));
                    node = node->middle;
                    currIdx++;
                }

                // Mark last letter as end of word and add the word's frequency
                node->isEndWord = true;
                node->freq = freq;
                return true;
            }

            // Checks middle child
        } else {
            // If current letter is the last letter, mark as end of word
            if (word.at(currIdx) == word.at(word.size() - 1) &&
                currIdx == word.size() - 1) {
                if (node->freq == 0) {
                    node->isEndWord = true;
                    node->freq = freq;
                }
                return true;
            } else {
                // If middle node exists, go to it and proceed to next letter
                if (node->middle != nullptr) {
                    node = node->middle;
                    currIdx++;
                } else {
                    // Create nodes for rest of letters in word with middle
                    // child
                    currIdx++;
                    for (auto i = currIdx; i < word.size(); i++) {
                        node->middle = new TSTNode(word.at(i));
                        node = node->middle;
                        currIdx++;
                    }

                    // Mark last letter as end of word and add the word's
                    // frequency
                    node->isEndWord = true;
                    node->freq = freq;
                    return true;
                }
            }
        }
    }
    // Return false if insertion is unsuccessful
    return false;
}

/* Finds a word in the Ternary Search Trie and returns true if the word is
 * found. Returns false otherwise.
 * word: string that is searched within the trie
 */
bool DictionaryTrie::find(string word) const {
    // If trie empty, return false
    if (root == nullptr) {
        return false;
    }

    TSTNode* node = root;

    // Keeps track of current letter within word
    unsigned int currIdx = 0;

    // Keep looping until word is found or reaches end of trie
    while (true) {
        if (!node) {
            return false;
        }
        // Checks left child
        if (word.at(currIdx) < node->data) {
            // If left node exists, go left
            if (node->left != nullptr) {
                node = node->left;
            } else {
                // If no node exists, word is not in trie, return false
                return false;
            }

            // Checks right child
        } else if (word.at(currIdx) > node->data) {
            // If right node exists, go right
            if (node->right != nullptr) {
                node = node->right;
            } else {
                // If no node exists, word is not in trie, return false
                return false;
            }

            // Checks middle child
        } else {
            // If the current letter is the last letter in the word, return true
            if (word.at(currIdx) == word.at(word.size() - 1) &&
                currIdx == word.size() - 1) {
                return true;
            } else {
                // If middle node exists, go middle, and increment to the
                // next letter
                if (node->middle != nullptr) {
                    node = node->middle;
                    currIdx++;
                } else {
                    // Word has not been found in trie
                    return false;
                }
            }
        }
    }

    // Returns false is find is unsuccessful
    return false;
}

/* Predicts numCompletion words with the given prefix
   prefix: beginning of word to be found
   numCompletions: maximum amount of words to be found
*/
vector<string> DictionaryTrie::predictCompletions(string prefix,
                                                  unsigned int numCompletions) {
    // Find last letter of prefix
    TSTNode* begin = findHelper(prefix, root, false, 0);

    // If beginning node does not exist, return empty vector
    if (begin == nullptr) {
        return {};
    }

    // Vector to store all possible matches
    vector<pair<int, string>> wordVector;

    // Call predict to traverse trie and add autocompleted words to wordVector
    predict(wordVector, begin->middle, "");

    // Insert the prefix to each of the words found using predict
    for (auto i = 0; i < wordVector.size(); i++) {
        wordVector[i].second.insert(0, prefix);
    }

    // If prefix itself is a word, add to wordVector
    if (begin->isEndWord) {
        wordVector.push_back(make_pair(begin->freq, prefix));
    }

    // Return empty vector if no words
    if (wordVector.size() == 0) {
        return {};
    }

    // Sort by frequencies, then alphabetically if equal
    sort(wordVector.begin(), wordVector.end(), [](auto& left, auto& right) {
        if (left.first != right.first) {
            return left.first > right.first;
        }
        return left.second < right.second;
    });

    // String vector containing only words
    vector<string> wordsWithPrefix;

    int iterations = numCompletions;

    // Make number of iterations equal to wordVector's size if smaller than
    // numCompletions
    if (numCompletions > wordVector.size() - 1) {
        iterations = wordVector.size();
    }

    // Insert words into wordsWithPrefix
    for (int i = 0; i < iterations; i++) {
        wordsWithPrefix.push_back(wordVector[i].second);
    }

    return wordsWithPrefix;
}

/* Predict completions with given word with wildcard underscores
   pattern: word with underscores to be autocompleted
   numCompletions: maximum amount of words to be found
*/
std::vector<string> DictionaryTrie::predictUnderscores(
    string pattern, unsigned int numCompletions) {
    // If root does not exist, return empty vector
    if (root == nullptr) {
        return {};
    }

    // Create wordVector storing freq and word
    vector<pair<int, string>> wordVector;

    // Call helper to traverse trie and find autocompleted words
    underscoreHelper(wordVector, root, "", 0, pattern);

    // If no words, return empty vector
    if (wordVector.size() == 0) {
        return {};
    }

    // Sort vector by frequency, then alphabetically if freq equal
    sort(wordVector.begin(), wordVector.end(), [](auto& left, auto& right) {
        if (left.first != right.first) {
            return left.first > right.first;
        }
        return left.second < right.second;
    });

    // Vector with valid words
    vector<string> results;

    int iterations = numCompletions;

    // Make number of iterations equal to wordVector's size if smaller than
    // numCompletions
    if (numCompletions > wordVector.size() - 1) {
        iterations = wordVector.size();
    }

    // Insert words into wordsWithPrefix
    for (int i = 0; i < iterations; i++) {
        results.push_back(wordVector[i].second);
    }

    return results;
}

/* Delete nodes of entire TST */
DictionaryTrie::~DictionaryTrie() { deleteAll(root); }

/* Helper method that finds the node of the last letter of word. If word is
 * not found, return nullptr.
 * word: word to be searched for in trie
 * node: beginning node of where to start searching
 * setFreq: if last node exists in trie, set its freq
 * freq: freq of word
 */
TSTNode* DictionaryTrie::findHelper(string word, TSTNode* node, bool setFreq,
                                    unsigned int freq) {
    if (node == nullptr) {
        return nullptr;
    }

    // Keeps track of current letter within word
    int currIdx = 0;

    // Keep looping until word is found or reaches end of trie
    while (true) {
        // Checks left child
        if (word.at(currIdx) < node->data) {
            // If left node exists, go left
            if (node->left != nullptr) {
                node = node->left;
            } else {
                // If no node exists, word is not in trie, return nullptr
                return nullptr;
            }

            // Checks right child
        } else if (word.at(currIdx) > node->data) {
            // If right node exists, go right
            if (node->right != nullptr) {
                node = node->right;
            } else {
                // If no node exists, word is not in trie, return nullptr
                return nullptr;
            }

            // Checks middle child
        } else {
            // If the current letter is the last letter in the word, return
            // the node
            if (word.at(currIdx) == word.at(word.size() - 1) &&
                currIdx == word.size() - 1) {
                if (node->isEndWord == false && setFreq == true) {
                    node->isEndWord = true;
                    node->freq = freq;
                    return node;
                }
                return node;
            } else {
                // If middle node exists, go middle, and increment to the
                // next letter
                if (node->middle != nullptr) {
                    node = node->middle;
                    currIdx++;
                } else {
                    // Word has not been found in trie
                    return nullptr;
                }
            }
        }
    }

    // Returns nullptr is find is unsuccessful
    return nullptr;
}

/* Recursively traverse through trie to predict words based on given prefix
   wordVector: vector to add valid words to
   node: node containing letter being checked
   curString: string containing letters of valid nodes
*/
void DictionaryTrie::predict(vector<pair<int, string>>& wordVector,
                             TSTNode* node, string curString) {
    // If node does not exist return
    if (node == nullptr) {
        return;
    }

    // If node has the flag of end word, add to wordVector
    if (node->isEndWord) {
        wordVector.push_back(make_pair(node->freq, curString + node->data));
    }

    // Traverse left if left node exists
    if (node->left != nullptr) {
        predict(wordVector, node->left, curString);
    }

    // Traverse middle if middle node exists and add its letter to curString
    if (node->middle != nullptr) {
        predict(wordVector, node->middle, curString + node->data);
    }

    // Traverse right if right node exists
    if (node->right != nullptr) {
        predict(wordVector, node->right, curString);
    }

    return;
}

/* Recursively traverse through trie to predict words based on given prefix with
   underscores
   wordVector: vector for words and freqencies to be added to
   node: node containing letter being checked
   curString: string containing letters of valid nodes
   curIdx: current index of the words being checked
   word: word to be searched in trie
*/
void DictionaryTrie::underscoreHelper(vector<pair<int, string>>& wordVector,
                                      TSTNode* node, string curString,
                                      int curIdx, string word) {
    // If curIdx about to go out of bounds, return
    if (curIdx > word.size() - 1) {
        return;
    }

    // If letter is a wildcard
    if (word.at(curIdx) == '_') {
        // If letter is end of word, add to wordVector
        if (node->isEndWord == true && curIdx == word.size() - 1) {
            wordVector.push_back(make_pair(node->freq, curString + node->data));
        }

        // Go left if left node exists
        if (node->left != nullptr) {
            underscoreHelper(wordVector, node->left, curString, curIdx, word);
        }

        // Go middle if middle node exists, add letter to curString, and
        // increment curIdx
        if (node->middle != nullptr) {
            underscoreHelper(wordVector, node->middle, curString + node->data,
                             curIdx + 1, word);
        }

        // Go right if right node exists
        if (node->right != nullptr) {
            underscoreHelper(wordVector, node->right, curString, curIdx, word);
        }
    }

    // If letter is not an underscore
    if (word.at(curIdx) != '_') {
        // If letter is end of word, add to wordVector
        if (node->isEndWord == true && curIdx == word.size() - 1 &&
            word.at(curIdx) == node->data) {
            wordVector.push_back(make_pair(node->freq, curString + node->data));
        } else {
            // If current letter is less than the node's letter go left
            if (node->left != nullptr && word.at(curIdx) < node->data) {
                underscoreHelper(wordVector, node->left, curString, curIdx,
                                 word);

                // If current letter equals the node's letter go middle, add
                // letter to curString, and increment curIdx
            } else if (node->middle != nullptr &&
                       word.at(curIdx) == node->data) {
                underscoreHelper(wordVector, node->middle,
                                 curString + node->data, curIdx + 1, word);

                // If current letter is greater than the node's letter go right
            } else {
                if (node->right != nullptr) {
                    underscoreHelper(wordVector, node->right, curString, curIdx,
                                     word);
                }
            }
        }
    }

    return;
}

/* Helper method to recursively delete nodes in trie
   node: node to be deleted
*/
void DictionaryTrie::deleteAll(TSTNode* node) {
    // If trie empty, return
    if (node == nullptr) {
        return;
    }

    // Recurse left to delete
    deleteAll(node->left);

    // Recurse right to delete
    deleteAll(node->right);

    // Delete node
    delete node;
}