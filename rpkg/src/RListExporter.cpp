#include "RListExporter.hpp"

#include <cpp11.hpp>
#include <iostream>  // For debug statements
#include <sstream>

#include "Entity.hpp"
#include "ParentIDCalculator.hpp"

namespace RedatamLib {
using std::ostringstream, std::endl;

ListExporter::ListExporter(const std::string& outputDirectory)
    : m_path(outputDirectory) {
  if ('/' != m_path.back()) {
    m_path.append("/");
  }
}

cpp11::list ListExporter::ExportAllR(
    const std::vector<Entity>& entities) const {
  cpp11::writable::list result;
  cpp11::writable::strings entityNames;
  for (const Entity& entity : entities) {
    std::string exportingEntityMgs = "Exporting " + entity.GetName() + "...";
    cpp11::message(exportingEntityMgs.c_str());

    cpp11::writable::list entityList;
    entityNames.push_back(entity.GetName());

    cpp11::writable::strings variableNames;

    // Add REF_ID and PARENT_REF_ID columns
    size_t numRows = entity.GetRowsCount();
    cpp11::writable::integers ref_id_vec(numRows);
    cpp11::writable::integers parent_ref_id_vec(numRows);
    ParentIDCalculator pIDCalc(const_cast<Entity*>(&entity));

    std::string ref_id_name = entity.GetName() + "_REF_ID";
    std::string parent_ref_id_name = entity.GetParentName() + "_REF_ID";

    for (size_t row = 0; row < numRows; ++row) {
      ref_id_vec[row] = row + 1;
      if (!entity.GetParentName().empty()) {
        parent_ref_id_vec[row] = pIDCalc.GetParentID(row + 1);
      }
    }

    entityList.push_back(ref_id_vec);
    variableNames.push_back(ref_id_name);

    if (!entity.GetParentName().empty()) {
      entityList.push_back(parent_ref_id_vec);
      variableNames.push_back(parent_ref_id_name);
    }

    // Add vectors for each variable
    for (const Variable& v : *(entity.GetVariables().get())) {
      try {
        switch (v.GetType()) {
          case BIN:
          case PCK:
          case INT:
          case LNG: {
            auto values =
                static_cast<std::vector<uint32_t>*>(v.GetValues().get());
            cpp11::writable::integers rvalues(numRows);
            for (size_t i = 0; i < numRows; i++) {
              rvalues[i] = values->at(i);
            }
            entityList.push_back(rvalues);
            break;
          }
          case CHR: {
            auto values =
                static_cast<std::vector<std::string>*>(v.GetValues().get());
            cpp11::writable::strings rvalues(numRows);
            for (size_t i = 0; i < numRows; i++) {
              rvalues[i] = values->at(i);
            }
            entityList.push_back(rvalues);
            break;
          }
          case DBL: {
            auto values =
                static_cast<std::vector<double>*>(v.GetValues().get());
            cpp11::writable::doubles rvalues(numRows);
            for (size_t i = 0; i < numRows; i++) {
              rvalues[i] = values->at(i);
            }
            entityList.push_back(rvalues);
            break;
          }
          default:
            std::string unknownTypeMsg = "Unknown variable type: " +
              v.GetName();
            cpp11::message(unknownTypeMsg.c_str());
            break;
        }
      } catch (const std::exception& e) {
        std::string errorExportingVariableMsg = "Error exporting variable: " +
          v.GetName() + " - " + e.what();
        cpp11::message(errorExportingVariableMsg.c_str());
      }

      variableNames.push_back(v.GetName());
    }

    if (variableNames.size() > 0) {
      entityList.names() = variableNames;
    }
    result.push_back(entityList);
  }
  result.names() = entityNames;
  return result;
}
}  // namespace RedatamLib
