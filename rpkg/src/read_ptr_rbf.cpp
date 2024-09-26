#include <cpp11.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "ByteArrayReader.hpp"
#include "Entity.hpp"
#include "ParentIDCalculator.hpp"
#include "Variable.hpp"
#include "utils.hpp"

using namespace cpp11;
using namespace std;
using namespace RedatamLib;

[[cpp11::register]] list parse_ptr_(const std::string& path) {
  try {
    std::cout << "Opening file: " << path << std::endl;
    ByteArrayReader reader(path);

    // Assuming reader handles opening errors internally
    std::cout << "Reading PTR data..." << std::endl;
    Entity entity;
    if (!entity.GetPTRData()) {  // Adjusted to call without parameters
      stop("Error: Failed to read PTR data.");
    }

    auto variables = entity.GetVariables();
    if (variables->empty()) {
      stop("Error: No variables found in entity.");
    }

    std::cout << "Parsing variables..." << std::endl;
    unordered_map<std::string, vector<double>> double_vars;
    unordered_map<std::string, vector<int>> int_vars;
    unordered_map<std::string, vector<std::string>> char_vars;

    for (auto& v : *variables) {  // Dereferencing here
      std::cout << "Parsing variable: " << v.GetName() << std::endl;
      switch (v.GetType()) {
        case BIN:
        case PCK:
        case INT:
        case LNG: {
          auto values_ptr = static_cast<vector<uint32_t>*>(v.GetValues().get());
          if (values_ptr && !values_ptr->empty()) {
            vector<uint32_t>& values = *values_ptr;
            int_vars[v.GetName()] = vector<int>(values.begin(), values.end());
          } else {
            stop("Error: Invalid integer data for variable " + v.GetName());
          }
          break;
        }
        case CHR: {
          auto values_ptr = static_cast<vector<string>*>(v.GetValues().get());
          if (values_ptr && !values_ptr->empty()) {
            vector<string>& values = *values_ptr;
            char_vars[v.GetName()] = values;
          } else {
            stop("Error: Invalid string data for variable " + v.GetName());
          }
          break;
        }
        case DBL: {
          auto values_ptr = static_cast<vector<double>*>(v.GetValues().get());
          if (values_ptr && !values_ptr->empty()) {
            vector<double>& values = *values_ptr;
            double_vars[v.GetName()] = values;
          } else {
            stop("Error: Invalid double data for variable " + v.GetName());
          }
          break;
        }
        default:
          stop("Error: Unknown variable type for variable " + v.GetName());
      }
    }

    std::cout << "Converting to R list..." << std::endl;
    writable::list result;

    for (const auto& [key, value] : double_vars) {
      result[key] = writable::doubles(value.begin(), value.end());
    }
    for (const auto& [key, value] : int_vars) {
      result[key] = writable::integers(value.begin(), value.end());
    }
    for (const auto& [key, value] : char_vars) {
      result[key] = writable::strings(value.begin(), value.end());
    }

    std::cout << "Returning result..." << std::endl;
    return result;

  } catch (const std::exception& e) {
    stop("Exception: " + std::string(e.what()));
  } catch (...) {
    stop("Unknown error occurred.");
  }
}
