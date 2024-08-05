/**
 * Tester File to test correctness of DictionaryTrie.cpp
 *
 * Author: James Chong
 * Email: j2chong@ucsd.edu
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "DictionaryTrie.hpp"
#include "util.hpp"

using namespace std;
using namespace testing;

/* Empty test */
TEST(DictTrieTests, EMPTY_TEST) {
    DictionaryTrie dict;

    // Find words in empty trie
    ASSERT_EQ(dict.find("hello"), false);
    ASSERT_EQ(dict.find("abrakadabra"), false);

    vector<string> vect = {};

    // Predict a word returning no results
    ASSERT_EQ(dict.predictCompletions("hello", 10), vect);

    // Insert an empty string, which fails
    ASSERT_EQ(dict.insert("", 0), false);
}

/* Test insertions and finds, predict for predictCompletions() */
TEST(DictTrieTests, TEST_PREDICT_COMPLETIONS) {
    DictionaryTrie dict;

    // Insert word
    ASSERT_EQ(dict.insert("journal", 20), true);

    // Check if inserting again changes trie
    ASSERT_EQ(dict.insert("journal", 10), false);

    // Check if word is found in trie
    ASSERT_EQ(dict.find("journal"), true);

    // Insert words to populate trie
    ASSERT_EQ(dict.insert("journalist", 30), true);
    ASSERT_EQ(dict.insert("journalize", 5), true);
    ASSERT_EQ(dict.insert("journalese", 20), true);
    ASSERT_EQ(dict.insert("journals", 100), true);

    // Check if words just inserted exist in trie
    ASSERT_EQ(dict.find("journal"), true);
    ASSERT_EQ(dict.find("journalist"), true);
    ASSERT_EQ(dict.find("journalize"), true);
    ASSERT_EQ(dict.find("journalese"), true);
    ASSERT_EQ(dict.find("journals"), true);

    // Expected result after predicting with prefix "journal"
    vector<string> vect = {"journals", "journalist", "journal", "journalese",
                           "journalize"};

    // Find autocompletions of prefix "journal"
    ASSERT_EQ(dict.predictCompletions("journal", 30), vect);

    // Check that predictCompletions returns only 1 result
    vector<string> vect1 = {"journals"};
    ASSERT_EQ(dict.predictCompletions("journal", 1), vect1);
}

/* Test insertions and predicts on words with underscores for
 * predictUnderscores() */
TEST(DictTrieTests, TEST_UNDERSCORES) {
    DictionaryTrie dict;

    // Insert words to populate trie
    ASSERT_EQ(dict.insert("cat", 1), true);
    ASSERT_EQ(dict.insert("car", 2), true);
    ASSERT_EQ(dict.insert("gin", 3), true);
    ASSERT_EQ(dict.insert("git", 4), true);
    ASSERT_EQ(dict.insert("get", 5), true);
    ASSERT_EQ(dict.insert("gem", 6), true);
    ASSERT_EQ(dict.insert("go", 7), true);
    ASSERT_EQ(dict.insert("god", 8), true);
    ASSERT_EQ(dict.insert("gov", 9), true);
    ASSERT_EQ(dict.insert("got", 10), true);
    ASSERT_EQ(dict.insert("gut", 11), true);

    // Check only one result returned after asking for one completion
    vector<string> vect1 = {"gut"};
    ASSERT_EQ(dict.predictUnderscores("g__", 1), vect1);

    // Check all three letter autocompleted words starting with "g"
    vector<string> vect2 = {"gut", "got", "gov", "god",
                            "gem", "get", "git", "gin"};
    ASSERT_EQ(dict.predictUnderscores("g__", 20), vect2);

    // Check all three letter autocompleted words ending with "t"
    vector<string> vect3 = {"gut", "got", "get", "git", "cat"};
    ASSERT_EQ(dict.predictUnderscores("__t", 10), vect3);

    // Check only one result returned after asking for one completion
    vector<string> vect4 = {"gut"};
    ASSERT_EQ(dict.predictUnderscores("__t", 1), vect4);

    // Check all three letter autocompleted words with "o" as middle letter
    vector<string> vect5 = {"got", "gov", "god"};
    ASSERT_EQ(dict.predictUnderscores("_o_", 10), vect5);

    // Check inserting word that already exists in trie
    ASSERT_EQ(dict.insert("g", 10), false);
}