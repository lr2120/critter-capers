#include "Species.h"

Species::Species(const std::string &taxon_name, const std::string &common_name)
        : taxon_name(taxon_name), common_name(common_name) {}

std::string Species::getTaxonName() const {
    return taxon_name;
}

std::string Species::getCommonName() const {
    return common_name;
}

std::string Species::toString() const {
    return "Taxon name: " + taxon_name + "\nCommon name: " + common_name + "\n";
}
