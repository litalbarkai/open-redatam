#ifndef RLISTEXPORTER_HPP
#define RLISTEXPORTER_HPP

#include <string>
#include <vector>

#include <cpp11/list.hpp>
#include <cpp11/strings.hpp>
#include <cpp11/doubles.hpp>
#include <cpp11/integers.hpp>
#include <cpp11/function.hpp> // cpp11::message

#include "Entity.hpp"

namespace RedatamLib {

class ListExporter {
 public:
  ListExporter(const std::string &outputDirectory);
  void ExportAll(std::vector<Entity> &entities);
  cpp11::list ExportAllR(const std::vector<Entity> &entities) const;

 private:
  std::string m_path;

  void ListEntity(Entity &entity, cpp11::writable::list &result) const;
  static void CreateVariablesLegend(Entity &e, cpp11::writable::list &result);
  static void CreateVariablesLabels(Entity &e, cpp11::writable::list &result);
  static void CreateVariablesData(Entity &e, cpp11::writable::list &result);
  static void AddVariableLabels(const Variable &v,
                                cpp11::writable::list &result,
                                cpp11::writable::strings &resultNames,
                                const std::string &entityName);
};

}  // namespace RedatamLib

#endif  // RLISTEXPORTER_HPP
