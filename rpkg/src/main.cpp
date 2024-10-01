#include <cpp11.hpp>   //  R interface
#include <filesystem>  //  exists, create_directories
#include <string>

#include "RedatamDatabase.hpp"

using namespace cpp11;
using std::string;
namespace fs = std::filesystem;

[[cpp11::register]] strings export_redatam_to_csv_(std::string dicFilePath,
                                                   std::string outputDirPath) {
  try {
    RedatamLib::RedatamDatabase db(dicFilePath);
    db.ExportCSVFiles(outputDirPath);
  } catch (const std::exception& e) {
    return cpp11::writable::strings({"Export failed"});
  }

  return cpp11::writable::strings({"Export successful"});
}

[[cpp11::register]] cpp11::list export_redatam_to_list_(
    std::string dicFilePath) {
  try {
    RedatamLib::RedatamDatabase db(dicFilePath);
    return db.ExportRLists();
  } catch (const std::exception& e) {
    cpp11::stop("Export failed: %s", e.what());
  }
}
