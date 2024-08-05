/*
 * movietraveler.cpp
 * Author: James Chong
 * Date: 11/27/19
 *
 * Program that outputs each edge connection within a the optimal path to
 * connect all nodes based on their weight
 */

#include <bits/stdc++.h>

#include "ActorGraph.cpp"
#include "ActorGraph.hpp"

#define LEFT_BRACKET "("
#define LEFT_ARROW ")<--["
#define MOVIE_DELIM "#@"
#define RIGHT_ARROW "]-->("
#define RIGHT_BRACKET ")"
#define NODE_CONNECTED "#NODE CONNECTED: "
#define EDGE_CHOSEN "#EDGE CHOSEN: "
#define TOTAL_EDGE_WEIGHTS "TOTAL EDGE WEIGHTS: "
#define ARG_TWO 2
#define HEADER "(actor)<--[movie#@year]-->(actor)"

using namespace std;

// Struct defining a disjoint set that utilizes find and union
struct DisjointSets {
    // Union by height
    vector<int> ranks;

    // Mapping of an index and actor node and its parents index
    unordered_map<int, pair<int, ActorNode*>> data;

    /**
     * Constructor that initializes a disjoint set
     *
     * actorGraph: graph of actor and movie nodes
     * dataMap: mapping of an index and actor nodes and its parents index
     */
    DisjointSets(ActorGraph& actorGraph,
                 unordered_map<int, pair<int, ActorNode*>> dataMap) {
        data = dataMap;

        // Initialize ranks vector to be same size as data, filled with zeros
        for (auto node : data) {
            ranks.push_back(0);
        }
    }

    /**
     * Uses path compression to find the sentinel node's index of a given index
     *
     * a: index of node whose parent to be found
     */
    int find(int a) {
        vector<int> path;

        // Traverse to parent
        while (data[a].first != -1) {
            path.push_back(a);
            a = data[a].first;
        }

        // Compress path
        path_compression(path, a);

        // Return sentinel index
        return a;
    }

    /**
     * Optimization that attaches each node along the path to the sentinel
     *
     * path: path of node that find was called on
     * sentinel: parent node of the path
     */
    void path_compression(vector<int> path, int sentinel) {
        // For each node on the path, attach to sentinel
        for (int node : path) {
            data[node].first = sentinel;
        }
    }

    /**
     * Attaches node of an index to another node of its index
     *
     * a: index of node
     * b: index of another node
     */
    void sentinel_union(int a, int b) {
        int sentinel_a = find(a);
        int sentinel_b = find(b);

        // Both sentinels not equal
        if (sentinel_a != sentinel_b) {
            // If b's rank higher, attach a to it
            if (ranks[sentinel_a] < ranks[sentinel_b]) {
                data[sentinel_a].first = sentinel_b;

                // If a's rank higher, attach b to it
            } else if (ranks[sentinel_a] > ranks[sentinel_b]) {
                data[sentinel_b].first = sentinel_a;

                // If both have same rank, attach a to b
            } else {
                data[sentinel_a].first = sentinel_b;
                ranks[sentinel_b] += 1;
            }
        }
    }
};

/**
 * Creates a minimum spanning tree that prints the optimal path to connect all
 * nodes in a graph
 *
 * actorGraph: graph of actor and movie nodes
 * ds: disjoint set to use find and union
 * data: map of an index and an actor node and its disjoint set value
 * indexMap: mapping of actor node and its index
 * outFile: file to be printed to
 */
void movieTraveler(ActorGraph& actorGraph, DisjointSets& ds,
                   unordered_map<int, pair<int, ActorNode*>> data,
                   unordered_map<ActorNode*, int> indexMap, ofstream& outFile) {
    // Number of actor nodes in graph
    int nodeCount = actorGraph.getNodeCount(actorGraph);

    // Number of edges connected is the number of nodes minus 1
    int allNodesConnected = nodeCount - 1;

    // Keeps track of total edge weight of MST
    int totalEdgeWeights = 0;

    // Increments number of edges
    int edgeCount = 0;

    // Vector of edges in graph
    vector<MovieNode*> edgeVect = actorGraph.getEdgeVect(actorGraph);

    // Sort edge vector by edgeWeight then by movie name
    sort(edgeVect.begin(), edgeVect.end(), EdgeWeight());

    int index = 0;

    // Loop until all nodes are connected or there are no more edges
    while ((edgeCount != allNodesConnected) && (index < edgeVect.size())) {
        // Find movie at index
        MovieNode* movie = edgeVect[index];

        // Increment
        index++;

        // For both actors being connected by the movie
        for (ActorNode* actorNodeOne : movie->actorVect) {
            for (ActorNode* actorNodeTwo : movie->actorVect) {
                // Find index of sentinel for first actor
                int sentinel_a = ds.find(indexMap.at(actorNodeOne));

                // FInd index of sentinel for second actor
                int sentinel_b = ds.find(indexMap.at(actorNodeTwo));

                // If they are not the same sentinel, union them
                if (sentinel_a != sentinel_b) {
                    // Union them together
                    ds.sentinel_union(sentinel_a, sentinel_b);

                    // Increment number of edges taken
                    edgeCount++;

                    // Add to the total edge weight of MST
                    totalEdgeWeights += movie->edgeWeight;

                    // Print movie and its two actors
                    outFile << LEFT_BRACKET + actorNodeOne->actorName +
                                   LEFT_ARROW + movie->movieName + MOVIE_DELIM;
                    outFile << movie->year;
                    outFile
                        << RIGHT_ARROW + actorNodeTwo->actorName + RIGHT_BRACKET
                        << endl;
                }
            }
        }
    }

    // Print number of nodes connected, number of edges chosen, and total edge
    // weight of MST
    outFile << NODE_CONNECTED;
    outFile << allNodesConnected + 1 << endl;
    outFile << EDGE_CHOSEN;
    outFile << edgeCount << endl;
    outFile << TOTAL_EDGE_WEIGHTS;
    outFile << totalEdgeWeights << endl;
}

/**
 * Main function that parses command line args and runs program
 *
 * argc: number of command line args
 * argv: array containing command line args
 */
int main(int argc, char* argv[]) {
    ActorGraph actorGraph;

    // Create actor graph with actor and movie nodes
    actorGraph.loadFromFile(actorGraph, argv[1], true);

    // Open output file
    ofstream outFile(argv[ARG_TWO]);

    // Print header to output file
    outFile << HEADER << endl;

    // Get map of index and its actor node and disjoint set value
    unordered_map<int, pair<int, ActorNode*>> data =
        actorGraph.getDSM(actorGraph);

    // Get index map
    unordered_map<ActorNode*, int> indexMap =
        actorGraph.getIndexMap(actorGraph);

    // Create disjoint set object
    DisjointSets ds(actorGraph, data);

    // Calculate optimal path to connect all nodes in graph
    movieTraveler(actorGraph, ds, data, indexMap, outFile);

    // Close output file
    outFile.close();
}