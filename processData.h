// processData.h
#pragma once
#include "Species.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

class processData {
public:

    struct SpeciesInfo {
        string taxon_name;
        string common_name;
    };

    // Links predator and prey by taxon_id (used to create out-degrees)
    unordered_map<int, vector<pair<string, int>>> predatorAdjacencyList;
    // Links prey and predator by taxon_id (used to create in-degrees)
    unordered_map<int, vector<pair<string, int>>> preyAdjacencyList;
    // Links species taxon_id with other species info
    unordered_map<int, SpeciesInfo> speciesInfoMap;

    // read both csv files and process them in adjacency lists
    void readCSV(const string &filename,
                 unordered_map<int, vector<pair<string, int>>> &predatorAdjacencyList,
                 unordered_map<int, vector<pair<string, int>>> &preyAdjacencyList,
                 unordered_map<int, SpeciesInfo> &speciesInfoMap);

    vector<pair<int, SpeciesInfo>> findTopPredatorsWithMaxPreyVariety(int top);
    int countUniquePreyWithDepth(int predatorTsn, int depth, const unordered_map<int, vector<pair<string, int>>> &predatorAdjacencyList);
    vector<pair<int, SpeciesInfo>> findTopPredatorsWithMaxPreyVarietyWithDepth(int top, int depth);
    vector<int> getPreyOf(int predatorTsn) const;
    SpeciesInfo getSpeciesInfo(int tsn) const;
};
