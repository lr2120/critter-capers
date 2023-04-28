// processData.cpp
#include "processData.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;
void processData::readCSV(const string &filename,
                          unordered_map<int, vector<pair<string, int>>> &predatorList,
                          unordered_map<int, vector<pair<string, int>>> &preyList,
                          unordered_map<int, processData::SpeciesInfo> &speciesMap) {

    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }

    string line;
    string source_tsn_str;
    string source_taxon_name;
    string source_common_name;
    string target_tsn_str;
    string target_taxon_name;
    string target_common_name;
    string interaction_type;

    // Read and ignore the header line
    getline(file, line);

    while (getline(file, line)) {
        stringstream lineStream(line);
        getline(lineStream, source_tsn_str, ',');
        getline(lineStream, source_taxon_name, ',');
        getline(lineStream, source_common_name, ',');
        getline(lineStream, target_tsn_str, ',');
        getline(lineStream, target_taxon_name, ',');
        getline(lineStream, target_common_name, ',');
        getline(lineStream, interaction_type, ',');

        int source_tsn = 0;
        int target_tsn = 0;

        // handles exception if record's tsn cannot be converted to
        try {
            source_tsn = stoi(source_tsn_str);
            target_tsn = stoi(target_tsn_str);
        } catch (const invalid_argument &e) {
            cerr << "Error: Unable to convert TSNs to int for line: " << line << endl;
            continue;
        }

        // Convert names to lowercase
        transform(source_taxon_name.begin(), source_taxon_name.end(), source_taxon_name.begin(), ::tolower);
        transform(source_common_name.begin(), source_common_name.end(), source_common_name.begin(), ::tolower);
        transform(target_taxon_name.begin(), target_taxon_name.end(), target_taxon_name.begin(), ::tolower);
        transform(target_common_name.begin(), target_common_name.end(), target_common_name.begin(), ::tolower);

        // Add species info to speciesInfoMap in lowercase
        speciesInfoMap[source_tsn] = {source_taxon_name, source_common_name};
        speciesInfoMap[target_tsn] = {target_taxon_name, target_common_name};

        if (interaction_type == "preysOn") {
            // Populates predatorAdjacencyList
            predatorAdjacencyList[source_tsn].emplace_back(target_taxon_name, target_tsn);
            // Populates preyAdjacencyList
            preyAdjacencyList[target_tsn].emplace_back(source_taxon_name, source_tsn);
        } else if (interaction_type == "predatedBy") {
            // Populates predatorAdjacencyList
            predatorAdjacencyList[target_tsn].emplace_back(source_taxon_name, source_tsn);
            // Populates preyAdjacencyList
            preyAdjacencyList[source_tsn].emplace_back(target_taxon_name, target_tsn);
        }
    }

    file.close();
}

vector<pair<int, processData::SpeciesInfo>> processData::findTopPredatorsWithMaxPreyVariety(int top) {
    vector<pair<int, int>> preyCounts;

    for (const auto &predator : predatorAdjacencyList) {
        int predatorTsn = predator.first;
        int preyCount = predator.second.size();
        preyCounts.emplace_back(predatorTsn, preyCount);
    }

    sort(preyCounts.begin(), preyCounts.end(),
              [](const auto &a, const auto &b) { return a.second > b.second; });

    vector<pair<int, SpeciesInfo>> topPredators;
    for (int i = 0; i < top && i < preyCounts.size(); ++i) {
        int predatorTsn = preyCounts[i].first;
        topPredators.emplace_back(predatorTsn, speciesInfoMap[predatorTsn]);
    }

    return topPredators;
}

// Helper function to count unique prey for a given predator up to a certain depth
int processData::countUniquePreyWithDepth(int predatorTsn, int depth, const unordered_map<int,
        vector<pair<string, int>>> &predatorAdjacencyList) {
    if (depth <= 0) return 0;

    unordered_set<int> uniquePrey;
    const auto &directPreyList = predatorAdjacencyList.at(predatorTsn);

    for (const auto &prey : directPreyList) {
        int preyTsn = prey.second;
        uniquePrey.insert(preyTsn);

        int subPreyCount = countUniquePreyWithDepth(preyTsn, depth - 1, predatorAdjacencyList);
        uniquePrey.insert(subPreyCount);
    }

    return uniquePrey.size();
}

vector<pair<int, processData::SpeciesInfo>> processData::findTopPredatorsWithMaxPreyVarietyWithDepth(int top, int depth) {
    vector<pair<int, int>> preyCounts;

    for (const auto &predator : predatorAdjacencyList) {
        int predatorTsn = predator.first;
        int preyCount = countUniquePreyWithDepth(predatorTsn, depth, predatorAdjacencyList);
        preyCounts.emplace_back(predatorTsn, preyCount);
    }

    sort(preyCounts.begin(), preyCounts.end(),
              [](const auto &a, const auto &b) { return a.second > b.second; });

    vector<pair<int, SpeciesInfo>> topPredators;
    for (int i = 0; i < top && i < preyCounts.size(); ++i) {
        int predatorTsn = preyCounts[i].first;
        topPredators.emplace_back(predatorTsn, speciesInfoMap[predatorTsn]);
    }

    return topPredators;
}

vector<int> processData::getPreyOf(int predatorTsn) const {
    vector<int> preyList;
    auto it = predatorAdjacencyList.find(predatorTsn);
    if (it != predatorAdjacencyList.end()) {
        for (const auto &prey : it->second) {
            preyList.push_back(prey.second);
        }
    }
    return preyList;
}

processData::SpeciesInfo processData::getSpeciesInfo(int tsn) const {
    auto it = speciesInfoMap.find(tsn);
    if (it != speciesInfoMap.end()) {
        return it->second;
    }
    return {"", ""}; // Return an empty SpeciesInfo if the TSN is not found
}