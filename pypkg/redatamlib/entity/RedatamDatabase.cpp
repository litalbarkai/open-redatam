#include <cctype>    // std::tolower
#include <stdexcept> // std::invalid_argument
#include <string>    // find_last_of, substr, npos
#include <pybind11/pybind11.h>

#include "FuzzyEntityParser.hpp"
#include "RedatamDatabase.hpp"
#include "XMLParser.hpp"
#include "utils.hpp" // ThrowIfBad, GetFileExtension
#include "PyDictExporter.hpp"

namespace RedatamLib {
using std::invalid_argument;
using std::string;
using std::vector;

RedatamDatabase::RedatamDatabase(const string &fileName) {
  pybind11::print("Opening dictionary file...");
  OpenDictionary(fileName);
}

pybind11::dict RedatamDatabase::ExportPyLists() const {
  ListExporter exporter("");
  return exporter.ExportAllPy(m_entities);
}

void RedatamDatabase::OpenDictionary(const string &fileName) {
  string ext = GetFileExtension(fileName);

  if (".dic" == ext) {
    FuzzyEntityParser parser(fileName);
    m_entities = parser.ParseEntities();
  } else if (".dicx" == ext) {
    XMLParser parser;
    m_entities = parser.ParseFile(fileName);
  } else {
    ThrowIfBad<invalid_argument>(
        false,
        invalid_argument(
            "Error: Dictionary file's extension must be .dic or .dicx ."));
  }
}
} // namespace RedatamLib
