#include <cpp11.hpp>
#include <string>
#include <vector>

#include "Entity.hpp"
#include "FuzzyEntityParser.hpp"
#include "Variable.hpp"

using namespace cpp11;

// taken from XMLExporter.cpp in the original C++ code
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

list convert_variable_to_list(const RedatamLib::Variable& variable) {
  writable::list var_list;
  var_list.push_back(cpp11::r_string(variable.GetName()));
  var_list.push_back(cpp11::r_string(variable.GetDescription()));
  var_list.push_back(cpp11::r_string(GetVarType(variable.GetType())));
  var_list.push_back(integers({static_cast<int>(variable.GetDataSize())}));

  // writable::list tag_list;
  // for (const auto& tag : variable.GetTags()) {
  //   writable::list tag_entry;
  //   tag_entry.push_back(cpp11::r_string(tag.first));   // Value
  //   tag_entry.push_back(cpp11::r_string(tag.second));  // Label
  //   tag_list.push_back(tag_entry);
  // }
  // var_list.push_back(tag_list);

  var_list.push_back(cpp11::r_string(variable.GetFilePath()));

  // Set names for the list elements
  // var_list.names() = {"name", "label", "type", "fieldSize", "tag", 
  //                     "filename", "valueLabels"};
  var_list.names() = {"name", "label", "type", "fieldSize", "filename",
                      "valueLabels"};

  return var_list;
}

list convert_entities_to_list(
    const std::vector<RedatamLib::Entity>& entities) {
  writable::list result;
  writable::strings entity_names;

  for (const auto& entity : entities) {
    writable::list entity_list;
    entity_list.push_back(cpp11::r_string(entity.GetName()));
    entity_list.push_back(cpp11::r_string(entity.GetPTRPath()));

    // Dereference the shared_ptr to access the vector of variables
    const auto& variables = *(entity.GetVariables());

    // Convert each variable within the entity
    writable::list variable_list;
    for (const auto& variable : variables) {
      variable_list.push_back(convert_variable_to_list(variable));
    }

    // Set names for the entity list elements
    entity_list.push_back(variable_list);
    entity_list.names() = {"name", "filename", "variables"};

    result.push_back(entity_list);
    entity_names.push_back(cpp11::r_string(entity.GetName()));
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
