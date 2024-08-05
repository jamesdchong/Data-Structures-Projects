#include <iostream>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "HCTree.hpp"

using namespace std;
using namespace testing;

class SimpleHCTreeFixture : public ::testing::Test {
  protected:
    HCTree tree;

  public:
    SimpleHCTreeFixture() {
        // initialization code here
        vector<unsigned int> freqs(256);
        freqs['a'] = 3;
        freqs['s'] = 18;
        freqs['m'] = 3;
        freqs['p'] = 6;
        tree.build(freqs);
    }
};

// Test pseudo compression
TEST_F(SimpleHCTreeFixture, TEST_ENCODE) {
    ostringstream os;
    tree.encode('m', os);
    ASSERT_EQ(os.str(), "001");
}

// Test pseudo decompression
TEST_F(SimpleHCTreeFixture, TEST_DECODE) {
    istringstream is("01");
    ASSERT_EQ(tree.decode(is), 'p');
}

// Test true compression
TEST_F(SimpleHCTreeFixture, TEST_BITENCODE) {
    stringstream ss;
    BitOutputStream bos(ss);

    tree.encode('m', bos);
    bos.flush();

    string bitsStr = "00100000";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    ASSERT_EQ(ss.get(), asciiVal);
}

// Test true decompression
TEST_F(SimpleHCTreeFixture, TEST_BITDECODE) {
    string bitsStr = "00100000";
    string ascii = string(1, stoi(bitsStr, nullptr, 2));

    stringstream ss;
    ss.str(ascii);
    BitInputStream bis(ss);

    ASSERT_EQ(tree.decode(bis), 'm');
}