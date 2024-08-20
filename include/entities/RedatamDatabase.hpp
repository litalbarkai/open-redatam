#ifndef REDATAMDATABASE_HPP
#define REDATAMDATABASE_HPP

#include <string>
#include <vector>

#include "Entity.hpp"

namespace RedatamLib
{
using std::string, std::vector;

class RedatamDatabase
{
public:
    // Throws invalid_argument
    explicit RedatamDatabase(const string& filename);
    ~RedatamDatabase();

private:
    vector<Entity> m_entities;

    void OpenDictionary(const string& filename);
};

} // namespace RedatamLib

#endif // REDATAMDATABASE_HPP