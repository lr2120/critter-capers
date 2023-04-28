#include "DepthFirstSearch.h"
using namespace std;

DepthFirstSearch::DepthFirstSearch(const Graph& graph) : graph(graph) {
    int n = graph.nodes.size();
    parent.resize(n, -1);
    visited.resize(n, false);
    discoveryOrder.resize(n);
    finishOrder.resize(n);

    for (const auto& nodeEntry : graph.nodes) {
        int i = nodeEntry.first;
        if (!visited[i]) {
            depthFirstSearch(i);
        }
    }
}

vector<int> DepthFirstSearch::depthFirstSearch(int current) {
    // Mark the current vertex visited.
    visited[current] = true;
    discoveryOrder[discoverIndex++] = current;

    // Examine each vertex adjacent to the current vertex.
    auto currentNode = graph.nodes.at(current);
    for (const auto& neighborNode : currentNode->preyOn) {
        int neighbor = neighborNode->tsn;

        // Process a neighbor that has not been visited.
        if (!visited[neighbor]) {
            // Insert (current, neighbor) into the depth-first search tree.
            parent[neighbor] = current;

            // Recursively apply the algorithm starting at neighbor.
            depthFirstSearch(neighbor);
        }
    }

    // Mark current finished.
    finishOrder[finishIndex++] = current;

    return parent;
}

const vector<int> & DepthFirstSearch::getParent() const {
    return parent;
}
