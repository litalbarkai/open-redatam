#ifndef PYDICTEXPORTER_HPP
#define PYDICTEXPORTER_HPP

#include "Entity.hpp"

#include <string>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace RedatamLib {
using pybind11::dict;
using std::string;
using std::vector;

class PyDictExporter {
 public:
  explicit PyDictExporter(const string &outputDirectory);
  dict ExportAllPy(const vector<Entity> &entities) const;

 private:
  string m_path;

  static std::string clean_string(const string &input);
  void AddVariableLabels(const Variable &v,
                         dict &result,
                         dict &resultNames,
                         const string &entityName) const;
};

}  // namespace RedatamLib
#endif  // PYDICTEXPORTER_HPP
