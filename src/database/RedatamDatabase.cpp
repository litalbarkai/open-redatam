#include "RedatamDatabase.hpp"

#include <string> // find_last_of, substr, npos

#include "CSVExporter.hpp"
#include "FuzzyEntityParser.hpp"
#include "RedatamDatabase.hpp"
#include "XMLExporter.hpp"
#include "XMLParser.hpp"
#include "utils/utils.hpp" // ThrowIfBad, GetFileExtension

namespace RedatamLib {

using std::invalid_argument;
using std::string;
using std::vector;

RedatamDatabase::RedatamDatabase(const string &fileName) {
  std::cout << "Opening dictionary file..." << std::endl;
  OpenDictionary(fileName);
}

void RedatamDatabase::ExportCSVFiles(const string &outputDir) {
  CSVExporter exporter(outputDir);
  exporter.ExportAll(m_entities);
}

void RedatamDatabase::ExportSummary(const string &outputDir) {
  XMLExporter exporter(outputDir);
  exporter.ExportSummary(m_entities);
}

void RedatamDatabase::OpenDictionary(const string &fileName) {
  string ext = GetFileExtension(fileName);

  if (ext == ".dic") {
    FuzzyEntityParser parser(fileName);
    m_entities = parser.ParseEntities();
  } else if (ext == ".dicx") {
    XMLParser parser;
    m_entities = parser.ParseFile(fileName);
  } else {
    ThrowIfBad<std::invalid_argument>(
        false, "Error: Dictionary file's extension must be .dic or .dicx.");
  }
}

} // namespace RedatamLib
