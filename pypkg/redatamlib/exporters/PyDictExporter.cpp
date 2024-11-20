#include "PyDictExporter.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <algorithm> // For replace
#include <sstream>
#include <string>
#include <vector>

#include "Entity.hpp"
#include "ParentIDCalculator.hpp"

namespace RedatamLib {
using pybind11::dict;
using pybind11::list;
using pybind11::print;
using pybind11::str;
using std::endl;
using std::exception;
using std::ostringstream;
using std::replace;
using std::string;
using std::transform;
using std::vector;

// Function to clean non-UTF-8 characters from a string
string PyDictExporter::clean_string(const string &input) {
  string output;
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

PyDictExporter::PyDictExporter(const string &outputDirectory)
    : m_path(outputDirectory) {
  if ('/' != m_path.back()) {
    m_path.append("/");
  }
}

dict PyDictExporter::ExportAllPy(const vector<Entity> &entities) const {
  dict result;

  for (const Entity &entity : entities) {
    string entityName = entity.GetName();
    transform(entityName.begin(), entityName.end(), entityName.begin(),
              ::tolower);

    string exportingEntityMsg = "Exporting " + entityName + "...";
    print(exportingEntityMsg);

    dict entityDict;
    dict variableNames;

    // Add REF_ID and PARENT_REF_ID columns
    size_t numRows = entity.GetRowsCount();
    list ref_id_list;
    list parent_ref_id_list;
    ParentIDCalculator pIDCalc(const_cast<Entity *>(&entity));

    str ref_id_name = entity.GetName() + "_REF_ID";
    str parent_ref_id_name = entity.GetParentName() + "_REF_ID";

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
        list rvalues;
        switch (v.GetType()) {
        case BIN:
        case PCK:
        case INT:
        case LNG: {
          auto values = static_cast<vector<uint32_t> *>(v.GetValues().get());
          for (size_t i = 0; i < numRows; i++) {
            rvalues.append(values->at(i));
          }
          entityDict[str(v.GetName())] = rvalues;
          break;
        }
        case CHR: {
          auto values = static_cast<vector<string> *>(v.GetValues().get());
          for (size_t i = 0; i < numRows; i++) {
            // Clean the string to remove non-UTF-8 characters
            string clean_str = clean_string(values->at(i));
            // Replace '\0' with ' '
            replace(clean_str.begin(), clean_str.end(), '\0', ' ');
            rvalues.append(clean_str);
          }
          entityDict[str(v.GetName())] = rvalues;
          break;
        }
        case DBL: {
          auto values = static_cast<vector<double> *>(v.GetValues().get());
          for (size_t i = 0; i < numRows; i++) {
            rvalues.append(values->at(i));
          }
          entityDict[str(v.GetName())] = rvalues;
          break;
        }
        default:
          string unknownTypeMsg = "Unknown variable type: " + v.GetName();
          print(unknownTypeMsg);
          break;
        }
      } catch (const exception &e) {
        string errorExportingVariableMsg =
            "Error exporting variable: " + v.GetName() + " - " + e.what();
        print(errorExportingVariableMsg);
      }

      variableNames[str(v.GetName())] = v.GetName();

      // Add variable labels to the main list
      AddVariableLabels(v, result, variableNames, entity.GetName());
    }

    result[str(entity.GetName())] = entityDict;
  }
  return result;
}

void PyDictExporter::AddVariableLabels(const Variable &v, dict &result,
                                       dict &resultNames,
                                       const string &entityName) const {
  if (!v.GetTags().empty()) {
    dict labelTable;
    list variableColumn;
    list meaningColumn;

    for (const Tag &t : v.GetTags()) {
      string clean_key = clean_string(t.first);
      string clean_value = clean_string(t.second);
      replace(clean_key.begin(), clean_key.end(), '\0', ' ');
      replace(clean_value.begin(), clean_value.end(), '\0', ' ');

      variableColumn.append(clean_key);
      meaningColumn.append(clean_value);
    }

    labelTable["variable"] = variableColumn;
    labelTable["meaning"] = meaningColumn;

    string tableName = entityName + "_LABELS_" + v.GetName();

    // Append the label table to the result dictionary
    result[str(tableName)] = labelTable;
  }
}
} // namespace RedatamLib
