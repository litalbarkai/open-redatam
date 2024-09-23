#include <cpp11.hpp>
#include <string>
#include <vector>

#include "Entity.hpp"
#include "FuzzyEntityParser.hpp"
#include "Variable.hpp"

using namespace cpp11;

// Function to get the variable type from RedatamLib::VarType
std::string GetVarType(RedatamLib::VarType type) {
  switch (type) {
    case RedatamLib::CHR:
      return "STRING";
    case RedatamLib::DBL:
      return "REAL";
    case RedatamLib::INT:
      return "INT16";
    case RedatamLib::LNG:
    case RedatamLib::BIN:
    case RedatamLib::PCK:
      return "INTEGER";
    case RedatamLib::NA:
    default:
      return "";
  }
}

// Function to convert value labels to an R-friendly list
list convert_value_labels_to_list(
    const std::vector<std::pair<std::string, std::string>>& tags) {
  writable::list value_labels_list;
  for (const auto& tag : tags) {
    writable::list value_label;
    value_label.push_back(writable::strings({tag.first}));   // Value
    value_label.push_back(writable::strings({tag.second}));  // Label
    value_labels_list.push_back(value_label);
  }
  return value_labels_list;
}

// Function to convert variables to an R-friendly list format
list convert_variable_to_list(const RedatamLib::Variable& variable) {
  writable::list var_list;

  var_list.push_back(writable::strings({variable.GetName()}));
  var_list.push_back(writable::strings({variable.GetDescription()}));
  var_list.push_back(writable::strings({GetVarType(variable.GetType())}));
  var_list.push_back(integers({static_cast<int>(variable.GetDataSize())}));
  var_list.push_back(integers({static_cast<int>(variable.GetDecimals())}));
  var_list.push_back(writable::strings({variable.GetFilter()}));
  var_list.push_back(writable::strings({variable.GetRange()}));
  // var_list.push_back(convert_value_labels_to_list(variable.GetTags()));
  var_list.push_back(writable::strings({variable.GetFilePath()}));

  var_list.names() = {"name", "label", "type", "fieldsize", "decimals",
                      "filter", "range", "valuelabels", "filename"};

  return var_list;
}

list convert_entities_to_list(const std::vector<RedatamLib::Entity>& entities) {
  writable::list result;
  writable::strings entity_names;

  for (const auto& entity : entities) {
    writable::list entity_list;
    entity_list.push_back(writable::strings({entity.GetName()}));
    entity_list.push_back(writable::strings({entity.GetPTRPath()}));

    writable::list variable_list;
    writable::strings variable_names;
    const auto& variables = *(entity.GetVariables());
    for (const auto& variable : variables) {
      list var_list = convert_variable_to_list(variable);
      variable_list.push_back(var_list);
      variable_names.push_back(var_list[0]);
    }
    variable_list.names() = variable_names;

    entity_list.push_back(variable_list);
    entity_list.names() = {"name", "filename", "variables"};

    result.push_back(entity_list);
    entity_names.push_back(entity.GetName());
  }

  result.names() = entity_names;

  return result;
}

[[cpp11::register]] list read_dic_(const std::string& filepath) {
  try {
    RedatamLib::FuzzyEntityParser parser(filepath);
    std::vector<RedatamLib::Entity> entities = parser.ParseEntities();
    return convert_entities_to_list(entities);
  } catch (const std::exception& e) {
    stop("Error reading DIC file: %s", e.what());
  }
}
