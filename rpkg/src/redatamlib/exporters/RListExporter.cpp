#include "RListExporter.hpp"

#include <algorithm>  // For std::replace
#include <sstream>

#include "Entity.hpp"
#include "ParentIDCalculator.hpp"

namespace RedatamLib {

using std::endl;
using std::string;
using std::ostringstream;
using std::transform;
using std::vector;
using namespace cpp11;

ListExporter::ListExporter(const string &outputDirectory)
    : m_path(outputDirectory) {
  if (!m_path.empty() && '/' != m_path.back()) {
    m_path.append("/");
  }
}

list ListExporter::ExportAllR(const std::vector<Entity> &entities) const {
  size_t numEntities = entities.size();
  writable::list result(numEntities);
  writable::strings resultNames(numEntities);

  for (size_t entityIndex = 0; entityIndex < numEntities; ++entityIndex) {
    const Entity &entity = entities[entityIndex];
    string entityName = entity.GetName();
    transform(entityName.begin(), entityName.end(), entityName.begin(), ::tolower);

    string exportingEntityMsg = "Exporting " + entityName + "...";
    message(exportingEntityMsg.c_str());

    size_t numVariables = entity.GetVariables()->size();
    writable::list entityList(numVariables + 2);  // +2 for REF_ID and PARENT_REF_ID
    writable::strings variableNames(numVariables + 2);

    // Add REF_ID and PARENT_REF_ID columns
    size_t numRows = entity.GetRowsCount();
    writable::integers ref_id_vec(numRows);
    writable::integers parent_ref_id_vec(numRows);
    ParentIDCalculator pIDCalc(const_cast<Entity *>(&entity));

    string ref_id_name = entity.GetName() + "_REF_ID";
    string parent_ref_id_name = entity.GetParentName() + "_REF_ID";

    for (size_t row = 0; row < numRows; ++row) {
      ref_id_vec[row] = row + 1;
      if (!entity.GetParentName().empty()) {
        parent_ref_id_vec[row] = pIDCalc.GetParentID(row + 1);
      }
    }

    entityList[0] = ref_id_vec;
    variableNames[0] = ref_id_name;

    if (!entity.GetParentName().empty()) {
      entityList[1] = parent_ref_id_vec;
      variableNames[1] = parent_ref_id_name;
    }

    // Add vectors for each variable
    for (size_t varIndex = 0; varIndex < numVariables; ++varIndex) {
      const Variable &v = entity.GetVariables()->at(varIndex);
      try {
        switch (v.GetType()) {
          case BIN:
          case PCK:
          case INT:
          case LNG: {
            vector<uint32_t> *values = static_cast<std::vector<uint32_t> *>(v.GetValues().get());
            writable::integers rvalues(numRows);
            for (size_t i = 0; i < numRows; i++) {
              rvalues[i] = values->at(i);
            }
            entityList[varIndex + 2] = rvalues;
            break;
          }
          case CHR: {
            vector<string> *values = static_cast<vector<string> *>(v.GetValues().get());
            writable::strings rvalues(numRows);
            for (size_t i = 0; i < numRows; i++) {
              // replace '\0' with ' '
              string clean_string = values->at(i);
              replace(clean_string.begin(), clean_string.end(), '\0', ' ');
              rvalues[i] = clean_string;
            }
            entityList[varIndex + 2] = rvalues;
            break;
          }
          case DBL: {
            vector<double> *values = static_cast<vector<double> *>(v.GetValues().get());
            writable::doubles rvalues(numRows);
            for (size_t i = 0; i < numRows; i++) {
              rvalues[i] = values->at(i);
            }
            entityList[varIndex + 2] = rvalues;
            break;
          }
          default:
            string unknownTypeMsg = "Unknown variable type: " + v.GetName();
            message(unknownTypeMsg.c_str());
            break;
        }
      } catch (const std::exception &e) {
        string errorExportingVariableMsg = "Error exporting variable: " + v.GetName() + " - " + e.what();
        message(errorExportingVariableMsg.c_str());
      }

      variableNames[varIndex + 2] = v.GetName();

      // Add variable labels to the main list
      AddVariableLabels(v, result, resultNames, entity.GetName());
    }

    if (variableNames.size() > 0) {
      entityList.names() = variableNames;
    }
    result[entityIndex] = entityList;
    resultNames[entityIndex] = entity.GetName();
  }
  result.names() = resultNames;
  return result;
}

void ListExporter::AddVariableLabels(const Variable &v,
                                     writable::list &result,
                                     writable::strings &resultNames,
                                     const string &entityName) {
  if (!v.GetTags().empty()) {
    writable::list labelTable;
    writable::strings variableColumn;
    writable::strings meaningColumn;

    for (const Tag &t : v.GetTags()) {
      string clean_key = t.first;
      string clean_value = t.second;
      replace(clean_key.begin(), clean_key.end(), '\0', ' ');
      replace(clean_value.begin(), clean_value.end(), '\0', ' ');

      variableColumn.push_back(r_string(clean_key));
      meaningColumn.push_back(r_string(clean_value));
    }

    labelTable.push_back(variableColumn);
    labelTable.push_back(meaningColumn);

    string variableColumnName = v.GetName();
    string meaningColumnName = v.GetName() + "_DESCRIPTION";
    writable::strings columnNames = {variableColumnName,
                                            meaningColumnName};
    labelTable.names() = columnNames;

    string tableName = entityName + "_LABELS_" + v.GetName();

    // Append the label table to the result list
    result.push_back(labelTable);

    // Append the table name to the list of names
    resultNames.push_back(tableName);
  }
}

}  // namespace RedatamLib
