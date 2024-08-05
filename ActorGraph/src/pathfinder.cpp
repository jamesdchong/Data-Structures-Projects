/*
 * pathfinder.cpp
 * Author: James Chong
 * Date: 11/27/19
 *
 * Program that prints the shortest path between two given actors
 */

#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>

#include "ActorGraph.cpp"
#include "ActorGraph.hpp"

#define LEFT_BRACKET "("
#define RIGHT_BRACKET ")"
#define LEFT_ARROW "--["
#define MOVIE_DELIM "#@"
#define RIGHT_ARROW "]-->"
#define WEIGHTED 'w'
#define UNWEIGHTED 'u'
#define ARG_TWO 2
#define ARG_THREE 3
#define ARG_FOUR 4
#define HEADER "(actor)--[movie#@year]-->(actor)--..."
#define TAB_CHAR '\t'
#define SIZE_OF_PAIR 2

using namespace std;

// Vector to reset all data fields within an actor node
vector<ActorNode*> resetVect;

/**
 * Finds the shortest path from a given starting actor to a given ending actor
 * and prints the path taken
 *
 * actorGraph: graph to traverse
 * startActor: actor to begin searching
 * endActor: actor to find
 */
ActorNode* shortestPath(ActorGraph& actorGraph, string startActor,
                        string endActor) {
    // Priority queue of two pairs containing an actor node and its distance
    priority_queue<pair<ActorNode*, int>, vector<pair<ActorNode*, int>>,
                   EdgeComparator>
        actorQueue;

    // Get the node corresponding the the first actor name
    ActorNode* firstActor = actorGraph.get(actorGraph, startActor);

    // Get the node corresponding the the final actor name
    ActorNode* finalActor = actorGraph.get(actorGraph, endActor);

    // Set the first actor's distance to zero
    firstActor->dist = 0;

    // Push the node and its distance
    actorQueue.push(make_pair(firstActor, firstActor->dist));

    // Push node to reset vector
    resetVect.push_back(firstActor);

    // Checker if path from start to final actor found
    bool pathFound = false;

    // Loop until queue empty
    while (!actorQueue.empty()) {
        // Get the highest priority node and its distance
        pair<ActorNode*, int> nodePair = actorQueue.top();
        ActorNode* node = nodePair.first;
        actorQueue.pop();

        // If node has not been visited
        if (node->done == false) {
            // Set it to true
            node->done = true;

            // For each of this actor's neighbors,
            for (MovieNode* movieNode : node->movieVect) {
                for (ActorNode* actorNode : movieNode->actorVect) {
                    // Set dist to the neighbor the node's current distance plus
                    // the movie's edge weight
                    int dist = node->dist + movieNode->edgeWeight;

                    // If this new dist is less than the node's
                    // distance, continue the path through this node
                    if (dist < actorNode->dist) {
                        // New node's previous is the movie
                        movieNode->prev = node;

                        // Movie's previous is the node before
                        actorNode->prev = movieNode;

                        // Distance is the new, shorter distance
                        actorNode->dist = dist;

                        // Push this node into the priority queue
                        actorQueue.push(make_pair(actorNode, actorNode->dist));

                        // Push into reset vector
                        resetVect.push_back(actorNode);
                    }

                    // If final actor is found, path found
                    if (actorNode == finalActor) {
                        pathFound = true;
                    }
                }
            }
        }
    }

    // Return final node if path found
    if (pathFound) {
        pathFound = false;
        return finalActor;
    }

    // If no path found, return nullptr
    return nullptr;
}

// Forward Declarations printActor, printMovie
void printActor(ofstream& outFile, ActorNode* node, ActorNode* start);
void printMovie(ofstream& outFile, MovieNode* node, ActorNode* start);

/**
 * Recursively prints an actor node and calls printMovie to print the next movie
 *
 * outFile: file to be written to
 * node: actorNode to be printed
 * start: first actor on path
 */
void printActor(ofstream& outFile, ActorNode* node, ActorNode* start) {
    // Keep traversing backwards on the path until the start node is found
    if (node != start) {
        printMovie(outFile, node->prev, start);
    }

    // Print the actor's name with its required format
    outFile << LEFT_BRACKET;
    outFile << node->actorName;
    outFile << RIGHT_BRACKET;
}

/**
 * Recursively prints a movie node and calls printActor to print the next actor
 *
 * outFile: file to be written to
 * node: movieNode to be printed
 * start: first actor on path
 */
void printMovie(ofstream& outFile, MovieNode* node, ActorNode* start) {
    // Traverse to the movie node's previous actor
    printActor(outFile, node->prev, start);

    // Print movie name along with its required format
    outFile << LEFT_ARROW;
    outFile << node->movieName;
    outFile << MOVIE_DELIM;
    outFile << node->year;
    outFile << RIGHT_ARROW;
}

/**
 * Main function of the program that parses command line input, builds graph,
 * and finds shortest path between two actors
 *
 * argc: number of command line arguments
 * argv: vector containing command line arguments
 */
int main(int argc, char* argv[]) {
    ActorGraph actorGraph;

    // If u flag, build unweighted graph
    if (*argv[ARG_TWO] == UNWEIGHTED) {
        actorGraph.loadFromFile(actorGraph, argv[1], false);
    }

    // If w flag, build weighted graph
    if (*argv[ARG_TWO] == WEIGHTED) {
        actorGraph.loadFromFile(actorGraph, argv[1], true);
    }

    // Open input file
    ifstream inFile(argv[ARG_THREE]);

    bool haveHeader = false;

    // Open output file
    ofstream outFile(argv[ARG_FOUR]);

    // Print header to output file
    outFile << HEADER << endl;

    // Loop until end of input file
    while (inFile) {
        string s;

        // Skip if empty line
        if (!getline(inFile, s)) {
            break;
        }

        // Skip if line is header
        if (!haveHeader) {
            haveHeader = true;
            continue;
        }

        istringstream ss(s);
        vector<string> actorPair;

        // Retrieve start actor name and end actor name
        while (ss) {
            string str;
            if (!getline(ss, str, TAB_CHAR)) {
                break;
            }
            actorPair.push_back(str);
        }

        if (actorPair.size() != SIZE_OF_PAIR) {
            continue;
        }

        // Start, end actor
        string startActor(actorPair[0]);
        string endActor(actorPair[1]);

        // Get node in graph corresponding to start actor's name
        ActorNode* startActorNode = actorGraph.get(actorGraph, startActor);

        // If start actor not in graph, print empty line
        if (startActorNode == nullptr) {
            outFile << endl;
            continue;
        }

        // Find the shortest path from the start node to the end node
        ActorNode* endActorNode =
            shortestPath(actorGraph, startActor, endActor);

        // Print path if end node is found
        if (endActorNode != nullptr) {
            printActor(outFile, endActorNode, startActorNode);
        }

        // Empty line
        outFile << endl;

        // FOr every node, reset all of its data fields
        for (int i = 0; i < resetVect.size(); i++) {
            ActorNode* node = resetVect[i];
            node->dist = numeric_limits<double>::infinity();
            node->prev = nullptr;
            node->done = false;
        }
    }

    // Close all files
    inFile.close();
    outFile.close();
}
