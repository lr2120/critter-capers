// BreadthFirstSearch.cpp
#include "BreadthFirstSearch.h"

BreadthFirstSearch::BreadthFirstSearch(const Graph& graph) : graph(graph) {}

std::vector<int> BreadthFirstSearch::breadthFirstSearch(int start, int i) {
    std::queue<int> theQueue;
    std::vector<int> bfsTraversal; // Add this line

    // Declare the parent vector and initialize its elements to -1.
    std::vector<int> parent(graph.nodes.size(), -1);

    // Declare the identified vector and initialize its elements to false.
    std::vector<bool> identified(graph.nodes.size(), false);

    // Mark the start vertex as identified and insert it into the queue.
    identified[start] = true;
    theQueue.push(start);

    // Perform breadth-first search until done.
    while (!theQueue.empty()) {
        // Take a vertex, current, out of the queue. (Begin visiting current).
        int current = theQueue.front();
        theQueue.pop();

        bfsTraversal.push_back(current);

        // Examine each vertex, neighbor, adjacent to current.
        auto currentNode = graph.nodes.at(current);
        for (const auto &neighborNode : currentNode->preyOn) {
            int neighbor = neighborNode->tsn;

            // If neighbor has not been identified.
            if (!identified[neighbor]) {
                // Mark it identified.
                identified[neighbor] = true;

                // Place it into the queue.
                theQueue.push(neighbor);

                // Insert the edge (current, neighbor) into the tree.
                parent[neighbor] = current;
            }
        }
        // Finished visiting current.
    }

    return bfsTraversal;
}
