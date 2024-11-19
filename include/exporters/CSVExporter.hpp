#ifndef REDATAMLIB_CSVEXPORTER_HPP
#define REDATAMLIB_CSVEXPORTER_HPP

#include <mutex>
#include <string>
#include <vector>

#include "Entity.hpp"
#include "Variable.hpp"

namespace RedatamLib {
using std::mutex;
using std::string;
using std::vector;

class CSVExporter {
public:
  explicit CSVExporter(const string &outputDirectory);
  ~CSVExporter() = default;

  CSVExporter(const CSVExporter &) = delete;
  CSVExporter &operator=(const CSVExporter &) = delete;

  void ExportAll(vector<Entity> &entities);

private:
  string m_path;
  mutable mutex m_mtx;

  void CreateVariablesLegend(Entity &e) const;
  void CreateVariablesLabels(Entity &e) const;
  void CreateVariablesData(Entity &e) const;
  void ThreadExport(size_t start, size_t end, vector<Entity> &entities) const;
};
} // namespace RedatamLib

#endif // REDATAMLIB_CSVEXPORTER_HPP
