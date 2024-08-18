#ifndef REDATAMDATABASE_HPP
#define REDATAMDATABASE_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include "Entity.hpp"
#include "CursorReader.hpp"

namespace RedatamLib
{
using std::string, std::vector, std::unordered_map;

class RedatamDatabase
{
public:
    // Throws invalid_argument
    explicit RedatamDatabase(const string& filename);
    ~RedatamDatabase();

    // long GetTotalDataItems();
    // long GetTotalRowsSize();
    // vector<Entity> GetEntitiesList();

private:
    // vector<Entity> m_entities;
    // unordered_map<string, CursorReader> m_cursorReaders;

    void OpenDictionary(const string& filename);
};

} // namespace RedatamLib

#endif // REDATAMDATABASE_HPP