/*
 * linkpredictor.cpp
 * Author: James Chong
 * Date: 11/27/19
 *
 * Program that finds actors that have already collaborated with a given actor
 * and finds actors that are likely to collaborate with this actor. The output
 * is directed to two output files.
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>

#include "ActorGraph.cpp"
#include "ActorGraph.hpp"

#define MAX_CANDIDATES 4
#define TAB_CHAR '\t'
#define INDEX_TWO 2
#define ARG_TWO 2
#define ARG_THREE 3
#define ARG_FOUR 4
#define HEADER "Actor1,Actor2,Actor3,Actor4"

using namespace std;

/**
 * Prints the 4 highest priority actor names to a file
 *
 * actorGraph: graph with actor and movie nodes
 * bestPredictions: priority queue containing the candidates to be printed
 * outFile: file to be printed to
 */
void printCandidates(
    ActorGraph& actorGraph,
    priority_queue<pair<ActorNode*, int>, vector<pair<ActorNode*, int>>,
                   PriorityComparator>
        bestPredictions,
    ofstream& outFile) {
    int count = 0;

    vector<ActorNode*> checkSameNode;

    bool sameNodes = false;

    // Print 4 highest priority candidates or until queue empty
    while (!bestPredictions.empty() && count < MAX_CANDIDATES) {
        pair<ActorNode*, int> nodePair = bestPredictions.top();
        ActorNode* node = nodePair.first;
        bestPredictions.pop();

        // Check if node printed already
        for (int i = 0; i < checkSameNode.size(); i++) {
            if (checkSameNode[i] == node) {
                sameNodes = true;
            }
        }

        if (sameNodes) {
            sameNodes = false;
            continue;
        }

        checkSameNode.push_back(node);

        count++;

        // Print actor name
        outFile << node->actorName + TAB_CHAR;
    }

    outFile << endl;

    // Empty rest of priority queue
    while (!bestPredictions.empty()) {
        bestPredictions.pop();
    }
}

/**
 * Function that finds actors that have collaborated with a given actor and
 * directs output to outFile
 *
 * actorGraph: graph to search
 * query: actor to find collaborated actors
 * outFile: file to direct output to
 */
void collaboratedActors(ActorGraph& actorGraph, ActorNode* query,
                        ofstream& outFile) {
    // Declare queue for query neighbors
    queue<ActorNode*> q;

    // Vector to check if string has already been added to queue
    vector<ActorNode*> checkNodes;

    // For every movie the actor is in, look at all of its actors
    for (MovieNode* movieNode : query->movieVect) {
        for (ActorNode* neighbor : movieNode->actorVect) {
            // Skip if neighbor found is the query
            if (neighbor == query) {
                continue;
            }

            // Add valid nodes to queue
            q.push(neighbor);
        }
    }

    // Priority queue for neighbors of query
    priority_queue<pair<ActorNode*, int>, vector<pair<ActorNode*, int>>,
                   PriorityComparator>
        bestPredictions;

    // Loop until queue is empty
    while (!q.empty()) {
        // Number of edges between query and common neighbor
        int queryToCommonNeighbor = 0;

        // Number of edges between candidate and common neighbor
        int candidateToCommonNeighbor = 0;

        // Counter for queryToCommonNeighbor
        int edgeCounterOne = 0;

        // Counter for candidateToCommonNeighbor
        int edgeCounterTwo = 0;

        // Priority of candidate
        int priority = 0;

        // Candidate is the highest priority node
        ActorNode* candidate = q.front();
        q.pop();

        // Vector of common nodes between query and candidate
        vector<tuple<ActorNode*, int, int>> commonNeighbors;

        // Checks if a triangle can be made between query, candidate, and common
        // neighbor
        bool foundQuery = false;

        // For each movie candidate actor is in, look at each of the movie's
        // actors
        for (MovieNode* edgeOne : candidate->movieVect) {
            for (ActorNode* commonNeighbor : edgeOne->actorVect) {
                // Skip if the common neighbor is the candidate or the query
                if (commonNeighbor == candidate || commonNeighbor == query) {
                    continue;
                }

                // Increment number of edges between candidate and common
                // neighbor
                edgeCounterOne++;

                // For each movie common neighbor is in, look at each actor in
                // that movie
                for (MovieNode* edgeTwo : commonNeighbor->movieVect) {
                    for (ActorNode* potentialQuery : edgeTwo->actorVect) {
                        // If edge between common neighbor and query exists,
                        // common neighbor is found
                        if (potentialQuery == query) {
                            // Increment number of edges between query and
                            // common neighbor
                            edgeCounterTwo++;
                            foundQuery = true;
                        }
                    }
                }

                // If triangle made, push common neighbor to vector to calculate
                // priority
                if (foundQuery) {
                    foundQuery = false;
                    commonNeighbors.push_back(make_tuple(
                        commonNeighbor, edgeCounterOne, edgeCounterTwo));
                }

                // Reset counters
                edgeCounterOne = 0;
                edgeCounterTwo = 0;
            }
        }

        // Calculate priority for candidate
        for (int i = 0; i < commonNeighbors.size(); i++) {
            candidateToCommonNeighbor = get<1>(commonNeighbors[i]);
            queryToCommonNeighbor = get<INDEX_TWO>(commonNeighbors[i]);

            // Priority is sum of each common neighbor's product of edges from
            // candidate to common neighbor and query to common neighbor
            priority += candidateToCommonNeighbor * queryToCommonNeighbor;
        }

        // Push candidate to priority queue
        bestPredictions.push(make_pair(candidate, priority));
    }

    printCandidates(actorGraph, bestPredictions, outFile);
}

/**
 * Function that finds actors that have not yet collaborated with a given actor
 * but are the most likely to be collaborated with
 *
 * actorGraph: graph to search
 * query: actor to find uncollaborated actors
 * outFile: file to direct output to
 */
void uncollaboratedActors(ActorGraph& actorGraph, ActorNode* query,
                          ofstream& outFile) {
    // Queue for query second neighbors
    queue<ActorNode*> q;

    // Checks if first neighbor already searched
    vector<ActorNode*> checkFirstNeighbors;

    // CHecks if seoncd neighbor already searched
    vector<ActorNode*> checkSecondNeighbors;

    // Node already checked
    bool nodeExists = false;

    // For every movie query actor in, look at each of its actors
    for (MovieNode* edgeOne : query->movieVect) {
        for (ActorNode* firstNeighbor : edgeOne->actorVect) {
            // Skip if actor is the query actor
            if (firstNeighbor == query) {
                continue;
            }

            // If first neighbor has been added as a second neighbor, delete it
            // from the second neighbor vector
            for (int i = 0; i < checkSecondNeighbors.size(); i++) {
                if (checkSecondNeighbors[i] == firstNeighbor) {
                    checkSecondNeighbors.erase(checkSecondNeighbors.begin() +
                                               i);
                }
            }

            // Add to first neighbor vector
            checkFirstNeighbors.push_back(firstNeighbor);

            // For each movie the first neighbor is in, look at all of its
            // actors
            for (MovieNode* edgeTwo : firstNeighbor->movieVect) {
                // Skip if first edge and second edge are the same
                if (edgeTwo == edgeOne) {
                    continue;
                }

                for (ActorNode* secondNeighbor : edgeTwo->actorVect) {
                    // Skip if second neighbor is the first neighbor or the
                    // query actor
                    if (secondNeighbor == firstNeighbor ||
                        secondNeighbor == query) {
                        continue;
                    }

                    // Skip if second neighbor is already a first neighbor
                    for (int i = 0; i < checkFirstNeighbors.size(); i++) {
                        if (checkFirstNeighbors[i] == secondNeighbor) {
                            nodeExists = true;
                        }
                    }

                    // Skip if second neighbor is already a second neighbor
                    for (int i = 0; i < checkSecondNeighbors.size(); i++) {
                        if (checkSecondNeighbors[i] == secondNeighbor) {
                            nodeExists = true;
                        }
                    }

                    if (nodeExists) {
                        nodeExists = false;
                        continue;
                    }

                    // Add to second neighbor vector
                    checkSecondNeighbors.push_back(secondNeighbor);

                    // Push to queue as a candidate
                    q.push(secondNeighbor);
                }
            }
        }
    }

    // Priority queue for second neighors of query
    priority_queue<pair<ActorNode*, int>, vector<pair<ActorNode*, int>>,
                   PriorityComparator>
        bestPredictions;

    // Loop until queue empty
    while (!q.empty()) {
        // Number of edges between query and common neighbor
        int queryToCN = 0;

        // Number of edges between candidate and common neighbor
        int CNtoCandidate = 0;

        // Counter for queryToCommonNeighbor
        int edgeCounterOne = 0;

        // Counter for candidateToCommonNeighbor
        int edgeCounterTwo = 0;

        // Priority of candidate
        int priority = 0;

        // Candidate is the highest priority node
        ActorNode* candidate = q.front();
        q.pop();

        // Check if second neighbor is still a second neighbor
        if (find(checkSecondNeighbors.begin(), checkSecondNeighbors.end(),
                 candidate) == checkSecondNeighbors.end()) {
            continue;
        }

        // Vector of common nodes between query and candidate
        vector<tuple<ActorNode*, int, int>> commonNeighbors;

        // Checks if there is a link between a query, commonNeighbor, and the
        // candidate
        bool foundCandidate = false;

        // For each movie query actor in, look at each of its actors
        for (MovieNode* edgeOne : query->movieVect) {
            for (ActorNode* commonNeighbor : edgeOne->actorVect) {
                // Increment number of edges between query and common neighbor
                edgeCounterOne++;

                // Skip if common neighbor is the query or the candidate
                if (commonNeighbor == query || commonNeighbor == candidate) {
                    edgeCounterOne = 0;
                    continue;
                }

                // For each movie common neighbor is in, look at each of its
                // actors
                for (MovieNode* edgeTwo : commonNeighbor->movieVect) {
                    for (ActorNode* potentialCandidate : edgeTwo->actorVect) {
                        // Candidate found
                        if (potentialCandidate == candidate) {
                            // If candidate already found and is valid,
                            // increment number of edges between common neighbor
                            // and candidate
                            for (int i = 0; i < commonNeighbors.size(); i++) {
                                if (get<0>(commonNeighbors[i]) == candidate) {
                                    get<INDEX_TWO>(commonNeighbors[i])++;
                                }
                            }
                            edgeCounterTwo++;
                            foundCandidate = true;
                        }
                    }
                }

                // If valid link between query and candidate, push to
                // commonNeighbors to calculate priority
                if (foundCandidate) {
                    foundCandidate = false;
                    commonNeighbors.push_back(make_tuple(
                        commonNeighbor, edgeCounterOne, edgeCounterTwo));
                }

                edgeCounterOne = 0;
                edgeCounterTwo = 0;
            }
        }

        // Calculate priority for candidate
        for (int i = 0; i < commonNeighbors.size(); i++) {
            queryToCN = get<1>(commonNeighbors[i]);
            CNtoCandidate = get<INDEX_TWO>(commonNeighbors[i]);
            priority += queryToCN * CNtoCandidate;
        }

        // Push candidate to priority queue
        bestPredictions.push(make_pair(candidate, priority));
    }

    printCandidates(actorGraph, bestPredictions, outFile);
}

/**
 * Main function of the program that parses command line input, builds
 * graph, and finds actors that have collaborated with and are likely to
 * collaborate with a given actor
 *
 * argc: number of command line arguments argv: vector
 * containing command line arguments
 */
int main(int argc, char* argv[]) {
    ActorGraph actorGraph;

    // Create actor graph with actor and movie nodes
    actorGraph.loadFromFile(actorGraph, argv[1], false);

    // Open input file
    ifstream inFile(argv[ARG_TWO]);

    bool haveHeader = false;

    // Open two output files
    ofstream collaborateOutFile(argv[ARG_THREE]);
    ofstream uncollaborateOutFile(argv[ARG_FOUR]);

    // Print header into each file
    collaborateOutFile << HEADER << endl;
    uncollaborateOutFile << HEADER << endl;

    // Loop until end of input file reached
    while (inFile) {
        string s;

        // Skip if empty line
        if (!getline(inFile, s)) {
            break;
        }

        // Skip if line is the header
        if (!haveHeader) {
            haveHeader = true;
            continue;
        }

        istringstream ss(s);
        string actor;

        // Read in actor name
        while (ss) {
            string str;
            if (!getline(ss, str, TAB_CHAR)) {
                break;
            }
            actor = str;
        }

        // Find actorNode corresponding to actor name
        ActorNode* startActorNode = actorGraph.get(actorGraph, actor);

        // Print blank line if actor node not found
        if (startActorNode == nullptr) {
            collaborateOutFile << endl;
            uncollaborateOutFile << endl;
            continue;
        }

        // Find collaborated actors
        collaboratedActors(actorGraph, startActorNode, collaborateOutFile);

        // Find actors most likely to collaborate
        uncollaboratedActors(actorGraph, startActorNode, uncollaborateOutFile);
    }

    // Close all files
    inFile.close();
    collaborateOutFile.close();
    uncollaborateOutFile.close();
}
