/*
 * ActorGraph.cpp
 * Author: James Chong
 * Date: 11/27/19
 *
 * Implementation file that builds the actor graph
 */

#include "ActorGraph.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#define TAB_CHAR '\t'
#define COLUMNS 3
#define ARG_TWO 2
#define CURR_YEAR 2019
#define READ_FAILURE "Failed to read "
#define FAILURE_PUNCT "!\n"

using namespace std;

/**
 * Constructor of the Actor graph
 */
ActorGraph::ActorGraph(void) {}

/**
 * Return the actorNode given the actor name
 *
 * actorGraph: graph of actor and movie nodes
 * actorName: actorNode to be returned
 */
ActorNode* ActorGraph::get(ActorGraph& actorGraph, string actorName) {
    // Iterator to search map for actor name
    unordered_map<string, ActorNode*>::const_iterator actorItr =
        actorMap.find(actorName);

    // Return actorNode that corresponds to the actor name
    if (actorItr != actorMap.end()) {
        return actorMap.at(actorName);
    }

    // Return nullptr if not found
    return nullptr;
}

/**
 * Returns the vector of edges in the graph
 *
 * actorGraph: graph of actor and movie nodes
 */
vector<MovieNode*> ActorGraph::getEdgeVect(ActorGraph& actorGraph) {
    return edgeVect;
}

/**
 * Returns the disjoint set map of the index mapping to a pair of the
 * disjoint set value and the actor node
 *
 * actorGraph: graph of actor and movie nodes
 */
unordered_map<int, pair<int, ActorNode*>> ActorGraph::getDSM(
    ActorGraph& actorGraph) {
    return disjointSetMap;
}

/**
 * Returns the index map mapping an actor node to an index
 *
 * actorGraph: graph of actor and movie nodes
 */
unordered_map<ActorNode*, int> ActorGraph::getIndexMap(ActorGraph& actorGraph) {
    return indexMap;
}

/**
 * Returns the number of nodes within actorGraph
 *
 * actorGraph: graph of actor and movie nodes
 */
int ActorGraph::getNodeCount(ActorGraph& actorGraph) { return nodeCount; }

/**
 * Load the graph from a tab-delimited file of actor->movie relationships.
 *
 * in_filename: input filename
 * use_weighted_edges: if true, compute edge weights as 1 + (2019 -
 * movie_year), otherwise all edge weights will be 1
 */
bool ActorGraph::loadFromFile(ActorGraph& actorGraph, const char* in_filename,
                              bool use_weighted_edges) {
    // Initialize the file stream
    ifstream infile(in_filename);

    // Skips header
    bool have_header = false;

    int mapCounter = 0;

    // keep reading lines until the end of file is reached
    while (infile) {
        string s;

        // get the next line
        if (!getline(infile, s)) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss(s);
        vector<string> record;

        while (ss) {
            string str;

            // get the next string before hitting a tab character and put it in
            // 'str'
            if (!getline(ss, str, TAB_CHAR)) break;
            record.push_back(str);
        }

        if (record.size() != COLUMNS) {
            // we should have exactly 3 columns
            continue;
        }

        string actor(record[0]);
        string movie_title(record[1]);
        int year = stoi(record[ARG_TWO]);

        bool actorExists = false;
        bool movieExists = false;

        pair<string, int> movie = make_pair(movie_title, year);

        unordered_map<string, ActorNode*>::const_iterator actorItr =
            actorMap.find(actor);

        // Check if actor exists in map
        if (actorItr != actorMap.end()) {
            actorExists = true;
        }

        // Check if movie with year exists in map
        if (movieMap.find(movie) != movieMap.end()) {
            movieExists = true;
        }

        // Link actor and movie if both exists
        if (actorExists && movieExists) {
            // Find actor and movie nodes
            ActorNode* actorNode = actorMap.at(actor);
            MovieNode* movieNode = movieMap.at(movie);

            // Link the two together
            actorNode->movieVect.push_back(movieNode);
            movieNode->actorVect.push_back(actorNode);
        }

        // Create movie node and link together
        if (actorExists && !movieExists) {
            // Find actor node
            ActorNode* actorNode = actorMap.at(actor);

            // Add actor node to actor vector
            vector<ActorNode*> actorVect;

            // Create new movie node with movie, year, and vector containing the
            // actor
            MovieNode* movieNode = new MovieNode(movie_title, year, actorVect);

            // Link them together
            movieNode->actorVect.push_back(actorNode);
            actorNode->movieVect.push_back(movieNode);

            // If using weighted edges, edge is how old the movie is, otherwise
            // edge is 1
            if (use_weighted_edges) {
                movieNode->edgeWeight = 1 + (CURR_YEAR - year);
            }

            edgeVect.push_back(movieNode);

            // Insert movie into movie map
            // movieMap.insert(make_pair(movie, movieNode));
            movieMap[movie] = movieNode;
        }

        // Create actor node and link together
        if (!actorExists && movieExists) {
            // Find movie node
            MovieNode* movieNode = movieMap.at(movie);

            // Add movie node to movie vector
            vector<MovieNode*> movieVect;

            // Create new actor node with actor and vector containing the movie
            ActorNode* actorNode = new ActorNode(actor, movieVect);

            // Link them together
            movieNode->actorVect.push_back(actorNode);
            actorNode->movieVect.push_back(movieNode);

            // Add actor node and its initial sentinel value of -1 to a hashmap
            disjointSetMap[mapCounter] = make_pair(-1, actorNode);

            // Set the actor node's value to an index in disjointSetMap
            indexMap[actorNode] = mapCounter;

            // One more actor node added to graph
            nodeCount++;

            // Increment indexMap's index
            mapCounter++;

            // Insert actor into actor map
            // actorMap.insert(make_pair(actor, actorNode));
            actorMap[actor] = actorNode;
        }

        // Create actor and movie nodes to link together
        if (!actorExists && !movieExists) {
            // Create empty movie vector and actor vector
            vector<MovieNode*> movieVect;
            vector<ActorNode*> actorVect;

            // Create new actor node with actor and empty vector
            ActorNode* actorNode = new ActorNode(actor, movieVect);

            // Create new movie node with movie, year, and empty vector
            MovieNode* movieNode = new MovieNode(movie_title, year, actorVect);

            // Link together
            actorNode->movieVect.push_back(movieNode);
            movieNode->actorVect.push_back(actorNode);

            // If using weighted edges, edge is how old the movie is, otherwise
            // edge is 1
            if (use_weighted_edges) {
                movieNode->edgeWeight = 1 + (CURR_YEAR - year);
            }

            edgeVect.push_back(movieNode);

            // Add actor node and its initial sentinel value of -1 to a hashmap
            disjointSetMap[mapCounter] = make_pair(-1, actorNode);

            // Set the actor node's value to an index in disjointSetMap
            indexMap[actorNode] = mapCounter;

            // Increment indexMap's index
            mapCounter++;

            // One more actor node added to graph
            nodeCount++;

            // Insert actor into actor map, and movie into movie map
            actorMap[actor] = actorNode;
            movieMap[movie] = movieNode;
        }
    }

    if (!infile.eof()) {
        cerr << READ_FAILURE << in_filename << FAILURE_PUNCT;
        return false;
    }
    infile.close();

    return true;
}

/**
 * Helper function to delete Actor graph
 */
void ActorGraph::deleteGraph() {
    // For every entry in actorMap, delete its actorNode
    for (auto entry : actorMap) {
        delete entry.second;
    }

    // For every entry in movieMap, delete its movieNode
    for (auto entry : movieMap) {
        delete entry.second;
    }
}

/**
 * Destructor of the Actor graph
 */
ActorGraph::~ActorGraph() { deleteGraph(); }