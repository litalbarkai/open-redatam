#include "RedatamDatabase.hpp"

#include <cctype>  // std::tolower
#include <string>  // find_last_of, substr, npos

#include "FuzzyEntityParser.hpp"
#include "RListExporter.hpp"
#include "XMLParser.hpp"
#include "utils.hpp"  // ThrowIfBad, GetFileExtension

namespace RedatamLib {
using std::invalid_argument;
using std::string;
using std::vector;

RedatamDatabase::RedatamDatabase(const string &fileName) {
  cpp11::message("Opening dictionary file...");
  OpenDictionary(fileName);
}

cpp11::list RedatamDatabase::ExportRLists() const {
  ListExporter exporter("");
  return exporter.ExportAllR(m_entities);
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
      false,
      "Error: Dictionary file's extension must be .dic or .dicx.");
  }
}
}  // namespace RedatamLib
