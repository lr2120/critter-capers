#pragma once
#include <string>

class Species {
public:
    Species(const std::string &taxon_name, const std::string &common_name);
    std::string getTaxonName() const;
    std::string getCommonName() const;
    std::string toString() const;

private:
    std::string taxon_name;
    std::string common_name;
};

