#include <cpp11.hpp>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "FuzzyVariableParser.hpp"

using namespace cpp11;

RedatamLib::VarType GetVarTypeFromString(const std::string& var_type_str) {
  static const std::unordered_map<std::string, RedatamLib::VarType> varTypeMap =
      {{"STRING", RedatamLib::CHR},
       {"REAL", RedatamLib::DBL},
       {"INT16", RedatamLib::INT},
       {"INTEGER", RedatamLib::LNG}};

  auto it = varTypeMap.find(var_type_str);
  if (it != varTypeMap.end()) {
    return it->second;
  } else {
    stop("Unknown variable type: %s", var_type_str.c_str());
  }
}

using ReadFunction = sexp (*)(std::ifstream&, size_t);

sexp readCHR(std::ifstream& file, size_t fieldsize) {
  std::vector<std::string> data;
  std::vector<char> buffer(fieldsize + 1, '\0');  // Fixed size buffer
  while (file.read(buffer.data(), fieldsize)) {
    data.push_back(std::string(buffer.data(), fieldsize));
  }
  return writable::strings(data.begin(), data.end());
}

sexp readINT(std::ifstream& file, size_t) {
  std::vector<short> data;
  short value;
  while (file.read(reinterpret_cast<char*>(&value), sizeof(short))) {
    data.push_back(value);
  }
  return writable::integers(data.begin(), data.end());
}

sexp readBIN(std::ifstream& file, size_t) {
  std::vector<int> data;
  int value;
  while (file.read(reinterpret_cast<char*>(&value), sizeof(int))) {
    data.push_back(value);
  }
  return writable::integers(data.begin(), data.end());
}

sexp readDBL(std::ifstream& file, size_t) {
  std::vector<double> data;
  double value;
  while (file.read(reinterpret_cast<char*>(&value), sizeof(double))) {
    data.push_back(value);
  }
  return writable::doubles(data.begin(), data.end());
}

std::unordered_map<RedatamLib::VarType, ReadFunction> readFunctionMap = {
    {RedatamLib::CHR, readCHR},
    {RedatamLib::INT, readINT},
    {RedatamLib::BIN, readBIN},
    {RedatamLib::DBL, readDBL}};

sexp read_variable_data(const std::string& filename, RedatamLib::VarType type,
                        size_t fieldsize) {
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    stop("Failed to open file: %s", filename.c_str());
  }

  auto it = readFunctionMap.find(type);
  if (it != readFunctionMap.end()) {
    return it->second(file, fieldsize);
  } else {
    stop("Unknown variable type");
  }
}

[[cpp11::register]] sexp read_rbf_(list variable_meta) {
  std::string filename = as_cpp<std::string>(variable_meta["filename"]);
  std::string var_type_str = as_cpp<std::string>(variable_meta["type"]);
  size_t fieldsize = as_cpp<int>(variable_meta["fieldsize"]);

  RedatamLib::VarType var_type = GetVarTypeFromString(var_type_str);

  return read_variable_data(filename, var_type, fieldsize);
}
