#include "RedatamDatabase.hpp"

#include <cctype>  //  std::tolower
#include <cpp11.hpp>
#include <stdexcept>  //  std::invalid_argument
#include <string>     //  find_last_of, substr, npos

#include "FuzzyEntityParser.hpp"
#include "RListExporter.hpp"
#include "XMLParser.hpp"
#include "utils.hpp"  //  ThrowIfBad, GetFileExtension

namespace RedatamLib {
using std::string;
using std::vector;
using std::invalid_argument;

RedatamDatabase::RedatamDatabase(const string& fileName) {
  cpp11::message("Opening dictionary file...");
  OpenDictionary(fileName);
}

cpp11::list RedatamDatabase::ExportRLists() const {
  ListExporter exporter("");
  return exporter.ExportAllR(m_entities);
}

void RedatamDatabase::OpenDictionary(const string& fileName) {
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
}  // namespace RedatamLib
