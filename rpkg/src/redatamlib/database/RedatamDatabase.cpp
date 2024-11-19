#include <string> // find_last_of, substr, npos

#include "FuzzyEntityParser.hpp"
#include "RListExporter.hpp"
#include "RedatamDatabase.hpp"
#include "XMLParser.hpp"
#include "utils/utils.hpp" // ThrowIfBad, GetFileExtension

namespace RedatamLib {

using cpp11::list;
using cpp11::message;
using std::invalid_argument;
using std::string;
using std::vector;

RedatamDatabase::RedatamDatabase(const string &fileName) {
  message("Opening dictionary file...");
  OpenDictionary(fileName);
}

list RedatamDatabase::ExportRLists() const {
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
    ThrowIfBad<invalid_argument>(
        false, "Error: Dictionary file's extension must be .dic or .dicx.");
  }
}

} // namespace RedatamLib
