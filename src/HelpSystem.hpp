
#ifndef HELPSYSTEM_HELPSYSTEM_HPP
#define HELPSYSTEM_HELPSYSTEM_HPP


#include <string>
#include <vector>
#include "types.hpp"


Entry initHelpSystem(const std::string& path);

std::vector<Entry *> searchEntries(Entry *entry, std::string searchword);

#endif //HELPSYSTEM_HELPSYSTEM_HPP
