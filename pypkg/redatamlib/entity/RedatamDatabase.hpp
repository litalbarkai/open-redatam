#ifndef REDATAMLIB_REDATAMDATABASE_HPP
#define REDATAMLIB_REDATAMDATABASE_HPP

#include <string>
#include <vector>
#include <pybind11/pybind11.h>

#include "Entity.hpp"

namespace RedatamLib {
using std::string;
using std::vector;

class RedatamDatabase {
public:
  // Throws invalid_argument
  explicit RedatamDatabase(const string &fileName);
  ~RedatamDatabase() = default;

  RedatamDatabase(const RedatamDatabase &) = delete;
  RedatamDatabase &operator=(const RedatamDatabase &) = delete;

  pybind11::dict ExportPyLists() const;

private:
  vector<Entity> m_entities;

  void OpenDictionary(const string &fileName);
};

} // namespace RedatamLib

#endif // REDATAMLIB_REDATAMDATABASE_HPP
