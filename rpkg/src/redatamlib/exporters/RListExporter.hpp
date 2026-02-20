#ifndef RLISTEXPORTER_HPP
#define RLISTEXPORTER_HPP

#include "Entity.hpp"

#include <string>
#include <vector>
#include <cpp4r/doubles.hpp>
#include <cpp4r/function.hpp> // cpp4r::message
#include <cpp4r/integers.hpp>
#include <cpp4r/list.hpp>
#include <cpp4r/strings.hpp>

namespace RedatamLib {

using std::string;
using std::vector;

class ListExporter {
public:
  ListExporter(const string &outputDirectory);
  void ExportAll(vector<Entity> &entities);
  cpp4r::list ExportAllR(const vector<Entity> &entities) const;

private:
  string m_path;

  void ListEntity(Entity &entity, cpp4r::writable::list &result) const;
  static void CreateVariablesLegend(Entity &e, cpp4r::writable::list &result);
  static void CreateVariablesLabels(Entity &e, cpp4r::writable::list &result);
  static void CreateVariablesData(Entity &e, cpp4r::writable::list &result);
  static void AddVariableLabels(const Variable &v,
                                cpp4r::writable::list &result,
                                cpp4r::writable::strings &resultNames,
                                const string &entityName);
};

} // namespace RedatamLib

#endif // RLISTEXPORTER_HPP
