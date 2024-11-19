#include <string>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "redatamlib/database/RedatamDatabase.hpp"

pybind11::dict export_redatam_to_dict(const std::string &dicFilePath) {
  try {
    RedatamLib::RedatamDatabase db(dicFilePath);
    return db.ExportPyLists();
  } catch (const std::exception &e) {
    throw std::runtime_error(std::string("Export failed: ") + e.what());
  }
}

PYBIND11_MODULE(_redatam, m) {
  m.def("export_redatam_to_dict", &export_redatam_to_dict,
        "Export Redatam data to list");
}
