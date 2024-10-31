#include <algorithm> // For std::replace
#include <sstream>
#include <string>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "PyDictExporter.hpp"

#include "Entity.hpp"
#include "ParentIDCalculator.hpp"

namespace RedatamLib {
using std::endl;
using std::ostringstream;

// Function to clean non-UTF-8 characters from a string
std::string ListExporter::clean_string(const std::string &input) {
  std::string output;
  for (char c : input) {
    if ((c & 0x80) == 0) { // ASCII character
      output += c;
    } else {
      // Replace non-UTF-8 character with a placeholder (e.g., '?')
      output += '?';
    }
  }
  return output;
}

// just to mimic the original CSVExporter
ListExporter::ListExporter(const std::string &outputDirectory)
    : m_path(outputDirectory) {
  if ('/' != m_path.back()) {
    m_path.append("/");
  }
}

pybind11::dict
ListExporter::ExportAllPy(const std::vector<Entity> &entities) const {
  pybind11::dict result;

  for (const Entity &entity : entities) {
    std::string entityName = entity.GetName();
    std::transform(entityName.begin(), entityName.end(), entityName.begin(),
                   ::tolower);

    std::string exportingEntityMsg = "Exporting " + entityName + "...";
    pybind11::print(exportingEntityMsg);

    pybind11::dict entityDict;
    pybind11::dict variableNames;

    // Add REF_ID and PARENT_REF_ID columns
    size_t numRows = entity.GetRowsCount();
    pybind11::list ref_id_list;
    pybind11::list parent_ref_id_list;
    ParentIDCalculator pIDCalc(const_cast<Entity *>(&entity));

    pybind11::str ref_id_name = entity.GetName() + "_REF_ID";
    pybind11::str parent_ref_id_name = entity.GetParentName() + "_REF_ID";

    for (size_t row = 0; row < numRows; ++row) {
      ref_id_list.append(row + 1);
      if (!entity.GetParentName().empty()) {
        parent_ref_id_list.append(pIDCalc.GetParentID(row + 1));
      }
    }

    entityDict[ref_id_name] = ref_id_list;
    variableNames[ref_id_name] = ref_id_name;

    if (!entity.GetParentName().empty()) {
      entityDict[parent_ref_id_name] = parent_ref_id_list;
      variableNames[parent_ref_id_name] = parent_ref_id_name;
    }

    // Add vectors for each variable
    for (const Variable &v : *(entity.GetVariables().get())) {
      try {
        pybind11::list rvalues;
        switch (v.GetType()) {
        case BIN:
        case PCK:
        case INT:
        case LNG: {
          auto values =
              static_cast<std::vector<uint32_t> *>(v.GetValues().get());
          for (size_t i = 0; i < numRows; i++) {
            rvalues.append(values->at(i));
          }
          entityDict[pybind11::str(v.GetName())] = rvalues;
          break;
        }
        case CHR: {
          auto values =
              static_cast<std::vector<std::string> *>(v.GetValues().get());
          for (size_t i = 0; i < numRows; i++) {
            // Clean the string to remove non-UTF-8 characters
            std::string clean_str = clean_string(values->at(i));
            // Replace '\0' with ' '
            std::replace(clean_str.begin(), clean_str.end(), '\0', ' ');
            rvalues.append(clean_str);
          }
          entityDict[pybind11::str(v.GetName())] = rvalues;
          break;
        }
        case DBL: {
          auto values = static_cast<std::vector<double> *>(v.GetValues().get());
          for (size_t i = 0; i < numRows; i++) {
            rvalues.append(values->at(i));
          }
          entityDict[pybind11::str(v.GetName())] = rvalues;
          break;
        }
        default:
          std::string unknownTypeMsg = "Unknown variable type: " + v.GetName();
          pybind11::print(unknownTypeMsg);
          break;
        }
      } catch (const std::exception &e) {
        std::string errorExportingVariableMsg =
            "Error exporting variable: " + v.GetName() + " - " + e.what();
        pybind11::print(errorExportingVariableMsg);
      }

      variableNames[pybind11::str(v.GetName())] = v.GetName();

      // Add variable labels to the main list
      AddVariableLabels(v, result, variableNames, entity.GetName());
    }

    result[pybind11::str(entity.GetName())] = entityDict;
  }
  return result;
}

void ListExporter::AddVariableLabels(const Variable &v, pybind11::dict &result,
                                     pybind11::dict &resultNames,
                                     const std::string &entityName) {
  if (!v.GetTags().empty()) {
    pybind11::dict labelTable;
    pybind11::list variableColumn;
    pybind11::list meaningColumn;

    for (const Tag &t : v.GetTags()) {
      std::string clean_key = clean_string(t.first);
      std::string clean_value = clean_string(t.second);
      std::replace(clean_key.begin(), clean_key.end(), '\0', ' ');
      std::replace(clean_value.begin(), clean_value.end(), '\0', ' ');

      variableColumn.append(clean_key);
      meaningColumn.append(clean_value);
    }

    labelTable["variable"] = variableColumn;
    labelTable["meaning"] = meaningColumn;

    std::string tableName = entityName + "_LABELS_" + v.GetName();

    // Append the label table to the result dictionary
    result[pybind11::str(tableName)] = labelTable;
  }
}
} // namespace RedatamLib
