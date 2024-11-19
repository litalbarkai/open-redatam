#ifndef REDATAMLIB_REDATAMDATABASE_HPP
#define REDATAMLIB_REDATAMDATABASE_HPP

#include "Entity.hpp"
#include <pybind11/pybind11.h>

namespace RedatamLib {
using pybind11::dict;
using std::string;
using std::vector;

class RedatamDatabase {
 public:
  explicit RedatamDatabase(const string &fileName);
  ~RedatamDatabase() = default;

  RedatamDatabase(const RedatamDatabase &) = delete;
  RedatamDatabase &operator=(const RedatamDatabase &) = delete;

  dict ExportPyLists() const;

 private:
  vector<Entity> m_entities;

  void OpenDictionary(const string &fileName);
};

}  // namespace RedatamLib

#endif  // REDATAMLIB_REDATAMDATABASE_HPP
