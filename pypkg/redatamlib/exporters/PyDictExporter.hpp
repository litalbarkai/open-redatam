#ifndef PYLISTEXPORTER_HPP
#define PYLISTEXPORTER_HPP

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <string>
#include <vector>

#include "Entity.hpp"

namespace RedatamLib {

class ListExporter {
 public:
  ListExporter(const std::string& outputDirectory);
  void ExportAll(std::vector<Entity>& entities);
  pybind11::dict ExportAllPy(const std::vector<Entity>& entities) const;

 private:
  std::string m_path;

  static std::string clean_string(const std::string& input);
  void ListEntity(Entity& entity, pybind11::dict& result) const;
  static void CreateVariablesLegend(Entity& e, pybind11::dict& result);
  static void CreateVariablesLabels(Entity& e, pybind11::dict& result);
  static void CreateVariablesData(Entity& e, pybind11::dict& result);
  static void AddVariableLabels(const Variable& v, pybind11::dict& result,
                                pybind11::dict& resultNames,
                                const std::string& entityName);
};

}  // namespace RedatamLib

#endif  // PYLISTEXPORTER_HPP
