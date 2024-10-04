#include <cpp11.hpp>   //  R interface
#include <string>

#include "redatamlib/RedatamDatabase.hpp"

using namespace cpp11;
using std::string;

[[cpp11::register]] cpp11::list export_redatam_to_list_(
    std::string dicFilePath) {
  try {
    RedatamLib::RedatamDatabase db(dicFilePath);
    return db.ExportRLists();
  } catch (const std::exception& e) {
    cpp11::stop("Export failed: %s", e.what());
  }
}
