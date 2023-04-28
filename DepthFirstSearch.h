#pragma once
#include <vector>
#include "Graph.h"
#include<algorithm>
using namespace std;

class DepthFirstSearch {
public:
    //constructor
    DepthFirstSearch(const Graph& graph);

    vector<int> depthFirstSearch(int start);
    const vector<int> &getParent() const;

private:
    const Graph& graph;
    vector<int> parent;
    vector<bool> visited;
    vector<int> discoveryOrder;
    vector<int> finishOrder;

    int discoverIndex = 0;
    int finishIndex = 0;
};
