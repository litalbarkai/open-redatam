#include "RedatamDatabase.hpp"
#include <cpp4r/function.hpp>

#include <string>

using namespace cpp4r;

[[cpp4r::register]] cpp4r::list
export_redatam_to_list_(std::string dicFilePath) {
  try {
    RedatamLib::RedatamDatabase db(dicFilePath);
    return db.ExportRLists();
  } catch (const std::exception &e) {
    std::string errorMsg = "Error: " + std::string(e.what());
    cpp4r::stop(errorMsg.c_str());
  }
}
