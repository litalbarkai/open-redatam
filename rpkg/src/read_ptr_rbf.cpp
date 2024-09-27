#include <cpp11.hpp>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "ByteArrayReader.hpp"
#include "Entity.hpp"
#include "FuzzyVariableParser.hpp"
#include "ParentIDCalculator.hpp"
#include "Variable.hpp"
#include "utils.hpp"

using namespace cpp11;
using namespace std;
using namespace RedatamLib;

std::mutex mtx;

void ThreadParse(std::mutex& mtx, size_t start, size_t end,
                 vector<Entity>& entities, const std::string& path) {
  ByteArrayReader reader(path);
  FuzzyVariableParser varParser(reader, FindRootPath(path));
  for (size_t i = start; i < end; ++i) {
    varParser.ParseAllVariables(entities);
  }
}

Entity initialize_entity(const std::string& path) {
  std::cout << "Opening file: " << path << std::endl;
  ByteArrayReader reader(path);

  std::cout << "Reading PTR data..." << std::endl;
  Entity entity("EntityName", "ParentName", "Description", path, {0, 0});
  ParentIDCalculator pIDCalc(&entity);

  return entity;
}

void parse_variables(Entity& entity, const std::string& path) {
  vector<Entity> entities = {entity};

  size_t numThreads = std::thread::hardware_concurrency();
  numThreads = std::min(entities.size(), numThreads);

  size_t chunkSize = entities.size() / numThreads;

  std::vector<std::thread> threads;
  for (size_t i = 0; i < numThreads; ++i) {
    size_t start = i * chunkSize;
    size_t end = (i == numThreads - 1) ? entities.size() : start + chunkSize;

    threads.push_back(std::thread(ThreadParse, std::ref(mtx), start, end,
                                  std::ref(entities), path));
  }

  for (auto& t : threads) {
    t.join();
  }

  // Attach parsed variables to the entity
  if (!entities.empty()) {
    std::cout << "Attaching variables to entity..." << std::endl;
    entity.AttachVariables(entities[0].GetVariables());
  } else {
    std::cout << "No entities found after parsing." << std::endl;
  }
}

writable::list convert_to_r_list(Entity& entity) {
  std::cout << "Checking variables..." << std::endl;
  auto variables_ptr = entity.GetVariables();
  if (!variables_ptr || variables_ptr->empty()) {
    stop("Error: No variables found in entity or variables pointer is null.");
  }

  std::cout << "Variables found: " << variables_ptr->size() << std::endl;

  writable::list result;

  for (Variable& v : *variables_ptr) {
    std::cout << "Variable: " << v.GetName() << std::endl;

    auto values_ptr = v.GetValues();
    if (!values_ptr) {
      stop("Error: Null values pointer for variable " + v.GetName());
    }

    switch (v.GetType()) {
      case BIN:
      case PCK:
      case INT:
      case LNG: {
        auto int_values_ptr = static_cast<vector<uint32_t>*>(values_ptr.get());
        if (int_values_ptr && !int_values_ptr->empty()) {
          writable::integers int_values(int_values_ptr->begin(),
                                        int_values_ptr->end());
          result[v.GetName()] = int_values;
        } else {
          stop("Error: Invalid integer data for variable " + v.GetName());
        }
        break;
      }
      case CHR: {
        auto char_values_ptr = static_cast<vector<string>*>(values_ptr.get());
        if (char_values_ptr && !char_values_ptr->empty()) {
          writable::strings char_values(char_values_ptr->begin(),
                                        char_values_ptr->end());
          result[v.GetName()] = char_values;
        } else {
          stop("Error: Invalid string data for variable " + v.GetName());
        }
        break;
      }
      default:
        stop("Error: Unsupported variable type.");
    }
  }

  return result;
}

[[cpp11::register]] list parse_ptr_(const std::string& path) {
  try {
    Entity entity = initialize_entity(path);
    parse_variables(entity, path);
    writable::list result = convert_to_r_list(entity);

    std::cout << "Returning result..." << std::endl;
    return result;

  } catch (const std::exception& e) {
    stop("Exception: " + std::string(e.what()));
  } catch (...) {
    stop("Unknown error occurred.");
  }
}