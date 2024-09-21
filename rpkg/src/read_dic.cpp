#include <cpp11.hpp>
#include <string>
#include <vector>

#include "Entity.hpp"
#include "FuzzyEntityParser.hpp"

using namespace cpp11;

// Helper function to convert variable details into an R-friendly list format
list convert_variable_to_list(const RedatamLib::Variable& variable) {
  writable::list var_list;
  var_list.push_back(cpp11::r_string(variable.GetName()));  // Variable name
  var_list.push_back(
      cpp11::r_string(variable.GetFilePath()));  // Variable file path

  // Set names for the list elements
  var_list.names() = {"name", "filePath"};

  return var_list;
}

// Helper function to convert entities into an R-friendly list format
list convert_entities_to_list(const std::vector<RedatamLib::Entity>& entities) {
  writable::list result;
  for (const auto& entity : entities) {
    writable::list entity_list;
    entity_list.push_back(cpp11::r_string(entity.GetName()));  // Entity name

    // Dereference the shared_ptr to access the vector of variables
    const auto& variables =
        *(entity.GetVariables());  // Dereference the shared_ptr

    // Convert each variable within the entity
    writable::list variable_list;
    for (const auto& variable : variables) {
      variable_list.push_back(
          convert_variable_to_list(variable));  // Convert variable
    }

    // Set names for the entity list elements
    entity_list.push_back(variable_list);  // Add variable list to the entity
    entity_list.names() = {"name", "variables"};  // Name the entity elements

    // Add the entity to the result with the entity's name as the list name
    result.push_back(entity_list);
  }

  return result;
}

// Efficient function to read and parse a DIC file using FuzzyEntityParser
[[cpp11::register]] list read_dic_(const std::string& filepath) {
  try {
    // Initialize the FuzzyEntityParser to parse the DIC file
    RedatamLib::FuzzyEntityParser parser(filepath);

    // Parse the entities from the DIC file
    std::vector<RedatamLib::Entity> entities = parser.ParseEntities();

    // Convert the parsed entities into an R list and return it
    return convert_entities_to_list(entities);

  } catch (const std::exception& e) {
    stop("Error reading DIC file: %s", e.what());
  }
}
