#include "RedatamDatabase.hpp"

#include <string> // find_last_of, substr, npos

#include "FuzzyEntityParser.hpp"
#include "PyDictExporter.hpp"
#include "XMLParser.hpp"
#include "utils/utils.hpp" // ThrowIfBad, GetFileExtension

namespace RedatamLib {

using pybind11::dict;
using pybind11::print;
using std::invalid_argument;
using std::string;
using std::vector;

RedatamDatabase::RedatamDatabase(const string &fileName) {
  print("Opening dictionary file...");
  OpenDictionary(fileName);
}

dict RedatamDatabase::ExportPyLists() const {
  PyDictExporter exporter("");
  return exporter.ExportAllPy(m_entities);
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
