#ifndef REDATAMLIB_REDATAMDATABASE_HPP
#define REDATAMLIB_REDATAMDATABASE_HPP

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

    RedatamDatabase(const RedatamDatabase&) = delete;
    RedatamDatabase& operator=(const RedatamDatabase&) = delete;

private:
    vector<Entity> m_entities;

    void OpenDictionary(const string& filename);
};

} // namespace RedatamLib

#endif // REDATAMLIB_REDATAMDATABASE_HPP