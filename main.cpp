#include "processData.h"
#include "ForceDirectedGraph.h"
#include "sfml.h"

int main() {
    // Create processData instance
    processData dataProcessor;

    // Read CSV files and populate adjacency lists
    dataProcessor.readCSV("data/predators.csv", dataProcessor.predatorAdjacencyList,
                          dataProcessor.preyAdjacencyList, dataProcessor.speciesInfoMap);

    // Create SFMLWindow instance and run it
    SFMLWindow window(dataProcessor);
    window.run();

    return 0;
}
