#include <string>

#include <cpp11/function.hpp>

#include "redatamlib/database/RedatamDatabase.hpp"

using namespace cpp11;

[[cpp11::register]] cpp11::list export_redatam_to_list_(std::string dicFilePath) {
  try {
    RedatamLib::RedatamDatabase db(dicFilePath);
    return db.ExportRLists();
  } catch (const std::exception &e) {
    std::string errorMsg = "Error: " + std::string(e.what());
    cpp11::stop(errorMsg.c_str());
  }
}
