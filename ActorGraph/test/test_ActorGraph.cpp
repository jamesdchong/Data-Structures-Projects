#include <gtest/gtest.h>
#include "ActorGraph.cpp"
#include "ActorGraph.hpp"

using namespace std;
using namespace testing;

TEST(ActorGraphTests, TEST) {
    vector<ActorNode*> actorVect;
    MovieNode* movieNode = new MovieNode("Endgame", 2019, actorVect);

    vector<MovieNode*> movieVect;
    movieVect.push_back(movieNode);

    ActorNode* actorNode = new ActorNode("Robert Downey Jr.", movieVect);

    ASSERT_EQ(actorNode->movieVect, movieVect);
}

TEST(ActorGraphTests, TEST_LOADGRAPH) {
    ActorGraph actorGraph;
    ASSERT_EQ(actorGraph.loadFromFile(
                  actorGraph, "test/test_files/imdb_small_sample.tsv", false),
              true);
}
