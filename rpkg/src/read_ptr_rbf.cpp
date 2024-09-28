#include <cpp11.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "Entity.hpp"
#include "FuzzyVariableParser.hpp"
#include "Variable.hpp"
#include "utils.hpp"

using namespace cpp11;
using namespace RedatamLib;

std::string VarTypeToString(VarType type) {
  switch (type) {
    case CHR:
      return "CHR";
    case DBL:
      return "DBL";
    case INT:
      return "INT";
    case LNG:
      return "LNG";
    case BIN:
      return "BIN";
    case PCK:
      return "PCK";
    default:
      return "UNKNOWN";
  }
}

void parse_variables_with_metadata(Entity& entity,
                                   const std::vector<std::string>& rbf_paths) {
  for (const auto& rbf_path : rbf_paths) {
    std::cout << "Parsing RBF file: " << rbf_path << std::endl;
    ByteArrayReader reader(rbf_path);
    FuzzyVariableParser varParser(reader, FindRootPath(rbf_path));
    std::vector<Entity> entities = {entity};
    varParser.ParseAllVariables(entities);
  }
}

[[cpp11::register]] SEXP print_rbf_paths_with_logging(
    const std::string& path, const std::vector<std::string>& rbf_paths,
    const std::string& entityName, const std::string& parentName) {
  try {
    std::cout << "Entity initialized: " << entityName << std::endl;
    Entity entity(entityName, parentName, "Description", path, {0, 0});
    std::cout << "Rows in entity: " << entity.GetRowsCount() << std::endl;

    parse_variables_with_metadata(entity, rbf_paths);

    auto variables = entity.GetVariables();
    if (variables && !variables->empty()) {
      cpp11::writable::list var_list;
      for (const auto& var : *variables) {
        cpp11::writable::list single_var;
        single_var["name"] = writable::strings({var.GetName()});
        single_var["type"] =
            writable::strings({VarTypeToString(var.GetType())});
        single_var["description"] = writable::strings({var.GetDescription()});

        var_list.push_back(single_var);
      }
      return var_list;
    } else {
      stop("No variables found for the entity.");
    }
  } catch (const std::exception& e) {
    stop("Exception: " + std::string(e.what()));
  } catch (...) {
    stop("Unknown error occurred.");
  }
}
