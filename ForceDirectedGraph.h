// ForceDirectedGraph.h
#pragma once

#include "processData.h"
#include <vector>
#include <utility>
#include <set>

class ForceDirectedGraph {
public:
    ForceDirectedGraph(const processData& inputData, int width, int height);
    ForceDirectedGraph();

    struct Node {
        int tsn;
        processData::SpeciesInfo info;
        double x;
        double y;
        double vx;
        double vy;
    };

    struct Edge {
        int source;
        int target;
    };


    std::vector<Edge> edges;


    void simulate(int iterations, double timestep, double repulsion, double spring_constant, double friction);

    void printPositions() const;

    void generateForceDirectedGraph(const processData &data, int window_width, int window_height);
    void renderGraph(const std::string &windowTitle);
    void createGraphFromTopPredators(const std::vector<std::pair<int, processData::SpeciesInfo>> &topPredators,
                                     int depth, int window_width, int window_height);

private:
    Node &findNode(int tsn);
    std::vector<Node> nodes;
    // Declare the addPreyToGraph function
    void addPreyToGraph(int predatorTsn, int depth, std::set<int> &uniqueTsnSet);
    // Declare dataProcessor as a member variable
    const processData dataProcessor;
    int window_width;
    int window_height;
};


