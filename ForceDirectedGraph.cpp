// ForceDirectedGraph.cpp
#include <SFML/Graphics.hpp>
#include "ForceDirectedGraph.h"
#include "processData.h"
#include "Species.h"
#include <stdexcept>
#include <random>
#include <iostream>
#include <set>

using namespace std;

// Constructor: Initializes the graph with the given processData
ForceDirectedGraph::ForceDirectedGraph()
        : dataProcessor(), window_width(0), window_height(0) {
    // initialize other members here
}
ForceDirectedGraph::ForceDirectedGraph(const processData &inputData, const int width, const int height)
        : dataProcessor(inputData), window_width(width), window_height(height) {
    // Create nodes from the speciesInfoMap in processData
    for (const auto &[tsn, info] : inputData.speciesInfoMap) {
        nodes.push_back({tsn, info, 0.0, 0.0, 0.0, 0.0});
    }

    // Initialize node positions randomly
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-1.0, 1.0);

    for (auto &node : nodes) {
        node.x = dis(gen);
        node.y = dis(gen);
    }

    // Store edges in a separate vector
    for (const auto &[source, targetList] : inputData.predatorAdjacencyList) {
        for (const auto &[_, target] : targetList) {
            edges.push_back({source, target});
        }
    }
    generateForceDirectedGraph(dataProcessor, window_width, window_height);
}


// Simulate the force-directed graph layout for a given number of iterations
void ForceDirectedGraph::simulate(int iterations, double timestep, double repulsion, double spring_constant, double friction) {
    for (int iter = 0; iter < iterations; ++iter) {
        // Calculate repulsive forces between all pairs of nodes
        for (auto &node1: nodes) {
            for (auto &node2: nodes) {
                if (&node1 != &node2) {
                    double dx = node1.x - node2.x;
                    double dy = node1.y - node2.y;
                    double distance = sqrt(dx * dx + dy * dy);
                    double force = repulsion / (distance * distance);

                    double fx = force * dx / distance;
                    double fy = force * dy / distance;

                    node1.vx += fx;
                    node1.vy += fy;
                }
            }
        }

        // Calculate spring forces for connected nodes (edges)
        for (const auto &[source, target]: edges) {
            Node &node1 = findNode(source);
            Node &node2 = findNode(target);

            double dx = node1.x - node2.x;
            double dy = node1.y - node2.y;
            double distance = sqrt(dx * dx + dy * dy);
            double force = -spring_constant * distance;

            double fx = force * dx / distance;
            double fy = force * dy / distance;

            node1.vx += fx;
            node1.vy += fy;
            node2.vx -= fx;
            node2.vy -= fy;
        }

        // Update positions based on velocities and apply friction
        for (auto &node: nodes) {
            node.x += node.vx * timestep;
            node.y += node.vy * timestep;

            node.vx *= (1 - friction);
            node.vy *= (1 - friction);
        }
    }
}

// Print node positions to console
void ForceDirectedGraph::printPositions() const {
    for (const auto &node: nodes) {
        cout << "TSN: " << node.tsn << ", "
             << "Taxon Name: " << node.info.taxon_name << ", "
             << "Common Name: " << node.info.common_name << ", "
             << "Position: (" << node.x << ", " << node.y << ")" << endl;
    }
}

ForceDirectedGraph::Node &ForceDirectedGraph::findNode(int tsn) {
    auto it = find_if(nodes.begin(), nodes.end(), [tsn](const Node &node) {
        return node.tsn == tsn;
    });

    if (it == nodes.end()) {
        throw runtime_error("Node not found for TSN: " + to_string(tsn));
    }

    return *it;
}

void ForceDirectedGraph::generateForceDirectedGraph(const processData &data, int window_width, int window_height) {
    // Create nodes from the speciesInfoMap in processData
    for (const auto &[tsn, info] : data.speciesInfoMap) {
        nodes.push_back({tsn, info, 0.0, 0.0, 0.0, 0.0});
    }

    // Initialize node positions randomly
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-1.0, 1.0);

    for (auto &node : nodes) {
        node.x = dis(gen);
        node.y = dis(gen);
    }

    // Store edges in a separate vector
    for (const auto &[source, targetList] : data.predatorAdjacencyList) {
        for (const auto &[_, target] : targetList) {
            edges.push_back({source, target});
        }
    }

    printPositions();
//    cout << "Nodes in graph: " << nodes.size() << endl;
//    cout << "Edges in graph: " << edges.size() << endl;
//
//    // Call the force-directed graph simulation
//    simulate(2000, 0.01, 500, 0.1, 0.01);
}

void ForceDirectedGraph::renderGraph(const std::string &windowTitle) {
    sf::RenderWindow window(sf::VideoMode(2400, 1600), windowTitle);

    // Create graphical representation of nodes and edges
    std::vector<sf::CircleShape> circleShapes;
    std::vector<sf::VertexArray> edgeShapes;

    for (const auto &node : nodes) {
        sf::CircleShape circle(5);
        circle.setFillColor(sf::Color::Red);
        circle.setPosition(node.x * 300 + window_width / 2, node.y * 300 + window_height / 2);
        circleShapes.push_back(circle);
    }

    for (const auto &edge : edges) {
        Node &sourceNode = findNode(edge.source);
        Node &targetNode = findNode(edge.target);

        sf::VertexArray line(sf::Lines, 2);
        line[0].position = sf::Vector2f(sourceNode.x * 300 + window_width / 2, sourceNode.y * 300 + window_height / 2);
        line[1].position = sf::Vector2f(targetNode.x * 300 + window_width / 2, targetNode.y * 300 + window_height / 2);
        line[0].color = sf::Color::White;
        line[1].color = sf::Color::White;

        edgeShapes.push_back(line);
    }

    // Main loop that renders the graph
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::White);

        // Print statement before the loop
        std::cout << "Drawing edges of the graph..." << std::endl;

        for (const auto &edgeShape : edgeShapes) {
            window.draw(edgeShape);
        }
        for (const auto &circleShape : circleShapes) {
            window.draw(circleShape);
        }

        // Print statement after the loop
        std::cout << "Finished drawing edges of the graph." << std::endl;
        window.display();
    }
}

// ForceDirectedGraph.cpp
void ForceDirectedGraph::createGraphFromTopPredators(const std::vector<std::pair<int,
        processData::SpeciesInfo>> &topPredators, int depth, int window_width, int window_height) {

    // Clear existing nodes and edges
    nodes.clear();
    edges.clear();

    // Create a temporary set to store unique TSNs
    std::set<int> uniqueTsnSet;

    // Loop through top predators and add them to the graph
    for (const auto &predator : topPredators) {
        int predatorTsn = predator.first;
        processData::SpeciesInfo predatorInfo = predator.second;
        nodes.push_back({predatorTsn, predatorInfo, 0, 0, 0, 0});
        uniqueTsnSet.insert(predatorTsn);

        // Add predator's prey to the graph
        addPreyToGraph(predatorTsn, depth, uniqueTsnSet);
    }

    // Update positions and velocities of the nodes
    for (Node &node : nodes) {
        node.x = (rand() % window_width) / 2;
        node.y = (rand() % window_height) / 2;
        node.vx = 0;
        node.vy = 0;
    }
}

// ForceDirectedGraph.cpp
void ForceDirectedGraph::addPreyToGraph(int predatorTsn, int depth, std::set<int> &uniqueTsnSet) {
    if (depth == 0) {
        return;
    }

    const auto &preyList = dataProcessor.getPreyOf(predatorTsn);

    for (int preyTsn : preyList) {
        if (uniqueTsnSet.count(preyTsn) == 0) {
            nodes.push_back({preyTsn, dataProcessor.getSpeciesInfo(preyTsn), 0, 0, 0, 0});
            uniqueTsnSet.insert(preyTsn);
        }

        edges.push_back({predatorTsn, preyTsn});
        addPreyToGraph(preyTsn, depth - 1, uniqueTsnSet);
    }
}




