#ifndef REDATAMLIB_REDATAMDATABASE_HPP
#define REDATAMLIB_REDATAMDATABASE_HPP

#include <cpp11.hpp>
#include <map>
#include <string>
#include <vector>

#include "Entity.hpp"

namespace RedatamLib {
using std::string, std::vector, std::map;

class RedatamDatabase {
 public:
  // Throws invalid_argument
  explicit RedatamDatabase(const string& fileName);
  ~RedatamDatabase() = default;

  RedatamDatabase(const RedatamDatabase&) = delete;
  RedatamDatabase& operator=(const RedatamDatabase&) = delete;

  void ExportCSVFiles(const string& outputDir);
  void ExportSummary(const string& outputDir);

  map<string, cpp11::data_frame> GetDataFrames();
  map<string, cpp11::data_frame> GetSummaryFrames();

 private:
  vector<Entity> m_entities;

  void OpenDictionary(const string& fileName);
};

}  // namespace RedatamLib

#endif  // REDATAMLIB_REDATAMDATABASE_HPP
