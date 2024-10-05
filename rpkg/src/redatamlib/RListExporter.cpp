#include "RListExporter.hpp"

#include <algorithm>  // For std::replace
#include <cpp11.hpp>
#include <sstream>

#include "Entity.hpp"
#include "ParentIDCalculator.hpp"

namespace RedatamLib {
using std::ostringstream;
using std::endl;

// just to mimic the original CSVExporter
ListExporter::ListExporter(const std::string& outputDirectory)
    : m_path(outputDirectory) {
  if ('/' != m_path.back()) {
    m_path.append("/");
  }
}

cpp11::list ListExporter::ExportAllR(
    const std::vector<Entity>& entities) const {
  cpp11::writable::list result;
  cpp11::writable::strings resultNames;

  for (const Entity& entity : entities) {
    std::string entityName = entity.GetName();
    std::transform(entityName.begin(), entityName.end(), entityName.begin(),
                   ::tolower);

    std::string exportingEntityMsg = "Exporting " + entityName + "...";
    cpp11::message(exportingEntityMsg.c_str());

    cpp11::writable::list entityList;
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
              // replace '\0' with ' '
              std::string clean_string = values->at(i);
              std::replace(clean_string.begin(), clean_string.end(), '\0', ' ');
              rvalues[i] = clean_string;
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
            std::string unknownTypeMsg =
                "Unknown variable type: " + v.GetName();
            cpp11::message(unknownTypeMsg.c_str());
            break;
        }
      } catch (const std::exception& e) {
        std::string errorExportingVariableMsg =
            "Error exporting variable: " + v.GetName() + " - " + e.what();
        cpp11::message(errorExportingVariableMsg.c_str());
      }

      variableNames.push_back(v.GetName());

      // Add variable labels to the main list
      AddVariableLabels(v, result, resultNames, entity.GetName());
    }

    if (variableNames.size() > 0) {
      entityList.names() = variableNames;
    }
    result.push_back(entityList);
    resultNames.push_back(entity.GetName());
  }
  result.names() = resultNames;
  return result;
}

void ListExporter::AddVariableLabels(const Variable& v,
                                     cpp11::writable::list& result,
                                     cpp11::writable::strings& resultNames,
                                     const std::string& entityName) {
  if (!v.GetTags().empty()) {
    cpp11::writable::list labelTable;
    cpp11::writable::strings variableColumn;
    cpp11::writable::strings meaningColumn;

    for (const Tag& t : v.GetTags()) {
      std::string clean_key = t.first;
      std::string clean_value = t.second;
      std::replace(clean_key.begin(), clean_key.end(), '\0', ' ');
      std::replace(clean_value.begin(), clean_value.end(), '\0', ' ');

      variableColumn.push_back(cpp11::r_string(clean_key));
      meaningColumn.push_back(cpp11::r_string(clean_value));
    }

    labelTable.push_back(variableColumn);
    labelTable.push_back(meaningColumn);

    std::string variableColumnName = v.GetName();
    std::string meaningColumnName = v.GetName() + "_DESCRIPTION";
    cpp11::writable::strings columnNames = {variableColumnName,
                                            meaningColumnName};
    labelTable.names() = columnNames;

    std::string tableName = entityName + "_LABELS_" + v.GetName();

    // Append the label table to the result list
    result.push_back(labelTable);

    // Append the table name to the list of names
    resultNames.push_back(tableName);
  }
}
}  // namespace RedatamLib
