//BreadthFirstSearch.h
#pragma once
#include <vector>
#include <queue>
#include "Graph.h"

class BreadthFirstSearch {
public:
    explicit BreadthFirstSearch(const Graph& graph);

    std::vector<int> breadthFirstSearch(int start, int i);

private:
    const Graph& graph;
};
