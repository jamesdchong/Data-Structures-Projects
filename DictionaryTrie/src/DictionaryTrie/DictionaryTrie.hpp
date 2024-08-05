/**
 * Developer file that defines the variables and methods to be included in
 * DictionaryTrie.cpp
 *
 * Author: James Chong
 * Email: j2chong@ucsd.edu
 */
#ifndef DICTIONARY_TRIE_HPP
#define DICTIONARY_TRIE_HPP

#include <iostream>
#include <stack>
#include <string>
#include <utility>
#include <vector>
#include "TSTNode.hpp"

using namespace std;

/**
 * The class for a dictionary ADT, implemented as either
 * a multi-way trie or a ternary search trie.
 */
class DictionaryTrie {
  private:
    /* Keeps track of the TST's root */
    TSTNode* root;

    /* Helper method that finds the node of the last letter of word. If word is
     * not found, return nullptr.
     * word: word to be searched for in trie
     * node: beginning node of where to start searching
     */
    TSTNode* findHelper(string word, TSTNode* root, bool setFreq,
                        unsigned int freq);

    /* Recursively traverse through trie to predict words based on given prefix
       wordVector: vector to add valid words to
       node: node containing letter being checked
       curString: string containing letters of valid nodes
    */
    void predict(vector<pair<int, string>>& wordVector, TSTNode* node,
                 string curString);

    /* Recursively traverse through trie to predict words based on given prefix
       with underscores
       wordVector: vector for words and freqencies to be added to
       node: node containing letter being checked
       curString: string containing letters of valid nodes
       curIdx: current index of the words being checked
       word: word to be searched in trie
    */
    void underscoreHelper(vector<pair<int, string>>& wordVector, TSTNode* node,
                          string curString, int curIdx, string word);

    /* Helper method to recursively delete nodes in trie
       node: node to be deleted
    */
    void deleteAll(TSTNode* node);

  public:
    /* Initializes an empty ternary search trie. */
    DictionaryTrie();

    /* Inserts a word into the trie and marks the last letter as the end of the
     * word along with its frequency. If an empty string is inserted or the word
     * already exists in the trie, this method returns false.
     * word: string to be inserted into the trie
     * freq: frequency this word appears
     */
    bool insert(string word, unsigned int freq);

    /* Finds a word in the Ternary Search Trie and returns true if the word is
     * found. Returns false otherwise.
     * word: string that is searched within the trie
     */
    bool find(string word) const;

    /* Predicts numCompletion words with the given prefix
       prefix: beginning of word to be found
       numCompletions: maximum amount of words to be found
    */
    vector<string> predictCompletions(string prefix,
                                      unsigned int numCompletions);

    /* Predict completions with given word with wildcard underscores
      pattern: word with underscores to be autocompleted
      numCompletions: maximum amount of words to be found
    */
    vector<string> predictUnderscores(string pattern,
                                      unsigned int numCompletions);

    /* Delete nodes of entire TST */
    ~DictionaryTrie();
};

#endif  // DICTIONARY_TRIE_HPP
