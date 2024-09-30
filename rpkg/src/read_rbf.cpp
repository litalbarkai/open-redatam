#include <cpp11.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "ByteArrayReader.hpp"
#include "FuzzyVariableParser.hpp"
#include "FuzzyEntityParser.hpp"
#include "Variable.hpp"
#include "ParentIDCalculator.hpp"

using namespace cpp11;

RedatamLib::VarType GetVarTypeFromString(const std::string& var_type_str) {
  static const std::unordered_map<std::string, RedatamLib::VarType> varTypeMap =
      {{"STRING", RedatamLib::CHR},  {"REAL", RedatamLib::DBL},
       {"INT16", RedatamLib::INT},   {"INTEGER", RedatamLib::LNG}};

  auto it = varTypeMap.find(var_type_str);
  if (it != varTypeMap.end()) {
    return it->second;
  } else {
    cpp11::stop("Unknown variable type: %s", var_type_str.c_str());
  }
}

sexp ConvertValuesToR(const RedatamLib::Variable& var) {
  if (var.GetType() == RedatamLib::CHR) {
    auto values = static_cast<std::vector<std::string>*>(var.GetValues().get());
    return writable::strings(values->begin(), values->end());
  } else if (var.GetType() == RedatamLib::INT ||
             var.GetType() == RedatamLib::LNG ||
             var.GetType() == RedatamLib::BIN ||
             var.GetType() == RedatamLib::PCK) {
    auto values = static_cast<std::vector<uint32_t>*>(var.GetValues().get());
    return writable::integers(values->begin(), values->end());
  } else if (var.GetType() == RedatamLib::DBL) {
    auto values = static_cast<std::vector<double>*>(var.GetValues().get());
    return writable::doubles(values->begin(), values->end());
  } else {
    stop("Unknown variable type.");
  }
}

list ConvertEntityValuesToR(RedatamLib::Entity& entity) {
  writable::list entity_data;
  writable::strings variable_names;

  entity_data.push_back(writable::integers({1}));  // Mocked region reference ID
  variable_names.push_back(entity.GetName() + "_REF_ID");

  if (!entity.GetParentName().empty()) {
    RedatamLib::ParentIDCalculator parentCalc(&entity);
    std::vector<int> parent_ids(entity.GetRowsCount());
    for (size_t row = 0; row < entity.GetRowsCount(); ++row) {
      parent_ids[row] = parentCalc.GetParentID(row);
    }
    entity_data.push_back(
        writable::integers(parent_ids.begin(), parent_ids.end()));
    variable_names.push_back(entity.GetParentName() + "_REF_ID");
  }

  auto variables = entity.GetVariables();
  for (const auto& var : *variables) {
    entity_data.push_back(ConvertValuesToR(var));
    variable_names.push_back(var.GetName());
  }

  entity_data.names() = variable_names;
  return entity_data;
}

sexp read_rbf(list variable) {
  std::string filename = as_cpp<std::string>(variable["filename"]);
  std::string var_type_str = as_cpp<std::string>(variable["type"]);
  size_t fieldsize = as_cpp<size_t>(variable["fieldsize"]);
  RedatamLib::VarType var_type = GetVarTypeFromString(var_type_str);

  RedatamLib::Variable var("variable_name", var_type, filename, fieldsize, "",
                           "", {}, "", 0);

  // if (type == RedatamLib::LNG) {
  //   fieldsize = 4;  // Force 4-byte size for 32-bit integers
  // }

  // if (type == RedatamLib::LNG && fieldsize == 2) {
  //   type = RedatamLib::INT;
  // }

  var.Parse();
  return ConvertValuesToR(var);
}

[[cpp11::register]] list read_entity_(list entity) {
  list variables = as_cpp<list>(entity["variables"]);
  writable::list out;
  for (R_xlen_t i = 0; i < variables.size(); i++) {
    list variable_i = variables[i];
    out.push_back(read_rbf(variable_i));
  }
  out.names() = variables.names();
  return out;
}
