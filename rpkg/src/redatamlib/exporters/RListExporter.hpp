#ifndef RLISTEXPORTER_HPP
#define RLISTEXPORTER_HPP

#include "Entity.hpp"

#include <string>
#include <vector>
#include <cpp11/doubles.hpp>
#include <cpp11/function.hpp> // cpp11::message
#include <cpp11/integers.hpp>
#include <cpp11/list.hpp>
#include <cpp11/strings.hpp>

namespace RedatamLib {

using std::string;
using std::vector;

class ListExporter {
public:
  ListExporter(const string &outputDirectory);
  void ExportAll(vector<Entity> &entities);
  cpp11::list ExportAllR(const vector<Entity> &entities) const;

private:
  string m_path;

  void ListEntity(Entity &entity, cpp11::writable::list &result) const;
  static void CreateVariablesLegend(Entity &e, cpp11::writable::list &result);
  static void CreateVariablesLabels(Entity &e, cpp11::writable::list &result);
  static void CreateVariablesData(Entity &e, cpp11::writable::list &result);
  static void AddVariableLabels(const Variable &v,
                                cpp11::writable::list &result,
                                cpp11::writable::strings &resultNames,
                                const string &entityName);
};

} // namespace RedatamLib

#endif // RLISTEXPORTER_HPP
