/*
 * ActorGraph.hpp
 * Author: James Chong
 * Date: 11/27/19
 *
 * Header File defining functions, variables, and structs to be implemented in
 * ActorGraph.cpp
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include <limits>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

class ActorNode;
class MovieNode;

// Struct that hashes pairs into an unordered map
struct HashPairs {
    size_t operator()(const pair<string, int>& pair) const {
        return hash<string>()(pair.first) ^ hash<int>()(pair.second);
    }
};

/**
 * Class that defines the actor graph that contains actor nodes with vectors of
 * movie nodes and movies nodes with vectors of actor nodes connecting each
 * other
 */
class ActorGraph {
  protected:
    // Map storing actors and their corresponding actorNode
    unordered_map<string, ActorNode*> actorMap;

    // Map storing movies and their corresponding movieNode
    unordered_map<pair<string, int>, MovieNode*, HashPairs> movieMap;

    // Vector of all movie edges
    vector<MovieNode*> edgeVect;

    // Map mapping an index to its disjoint set value, and the node itself
    unordered_map<int, pair<int, ActorNode*>> disjointSetMap;

    // Map mapping actor node to an index
    unordered_map<ActorNode*, int> indexMap;

    // Number of nodes in the graph
    int nodeCount;

  public:
    /**
     * Constuctor of the Actor graph
     */
    ActorGraph(void);

    /**
     * Destructor of the Actor graph
     */
    ~ActorGraph();

    /**
     * Helper function to delete Actor graph
     */
    void deleteGraph();

    /**
     * Return the actorNode given the actor name
     *
     * actorGraph: graph of actor and movie nodes
     * actorName: actorNode to be returned
     */
    ActorNode* get(ActorGraph& actorGraph, string actorName);

    /**
     * Returns the vector of edges in the graph
     *
     * actorGraph: graph of actor and movie nodes
     */
    vector<MovieNode*> getEdgeVect(ActorGraph& actorGraph);

    /**
     * Returns the disjoint set map of the index mapping to a pair of the
     * disjoint set value and the actor node
     *
     * actorGraph: graph of actor and movie nodes
     */
    unordered_map<int, pair<int, ActorNode*>> getDSM(ActorGraph& actorGraph);

    /**
     * Returns the index map mapping an actor node to an index
     *
     * actorGraph: graph of actor and movie nodes
     */
    unordered_map<ActorNode*, int> getIndexMap(ActorGraph& actorGraph);

    /**
     * Returns the number of nodes within actorGraph
     *
     * actorGraph: graph of actor and movie nodes
     */
    int getNodeCount(ActorGraph& actorGraph);

    /**
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename: input filename
     * use_weighted_edges: if true, compute edge weights as 1 + (2019 -
     * movie_year), otherwise all edge weights will be 1
     */
    bool loadFromFile(ActorGraph& actorGraph, const char* in_filename,
                      bool use_weighted_edges);
};

// Defines an actorNode
struct ActorNode {
    // Name of actor
    string actorName;

    // Vector containing movies the actor has been in
    vector<MovieNode*> movieVect;

    // Initialize dist to infinity
    double dist;

    // Initialize prev to nullptr
    MovieNode* prev;

    // Initialize done to false
    bool done;

    ActorNode(string actorName, vector<MovieNode*> movieVect)
        : actorName(actorName), movieVect(movieVect) {
        dist = numeric_limits<double>::infinity();
        prev = nullptr;
        done = false;
    }
};

// Defines a movie node
struct MovieNode {
    // Name of movie
    string movieName;

    // Year of movie
    int year;

    // Vector containing actors in the movie
    vector<ActorNode*> actorVect;

    // Initialize prev to nullptr
    ActorNode* prev;

    // Initialize edgeWeight to 1
    int edgeWeight;

    MovieNode(string movieName, int year, vector<ActorNode*> actorVect)
        : movieName(movieName), year(year), actorVect(actorVect) {
        prev = nullptr;
        edgeWeight = 1;
    }
};

// Comparator that sorts by edgeWeight then by movie name
struct EdgeWeight {
    bool operator()(MovieNode*& lhs, MovieNode*& rhs) {
        if (lhs->edgeWeight != rhs->edgeWeight) {
            return lhs->edgeWeight < rhs->edgeWeight;
        }

        return lhs->movieName < rhs->movieName;
    }
};

// Comparator that sorts by lowest distance then by actor name
struct EdgeComparator {
    bool operator()(pair<ActorNode*, int>& lhs,
                    pair<ActorNode*, int>& rhs) const {
        if (lhs.second != rhs.second) {
            return lhs.second > rhs.second;
        }

        return lhs.first->actorName > rhs.first->actorName;
    }
};

// Comparator that sorts by highest priority then by actor name
struct PriorityComparator {
    bool operator()(pair<ActorNode*, int>& lhs, pair<ActorNode*, int>& rhs) {
        if (lhs.second != rhs.second) {
            return lhs.second < rhs.second;
        }
        return lhs.first->actorName > rhs.first->actorName;
    }
};
#endif  // ACTORGRAPH_HPP
