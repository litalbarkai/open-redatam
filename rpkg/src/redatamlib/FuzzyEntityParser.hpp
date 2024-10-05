//  replaces DataBlock.cs
#ifndef REDATAMLIB_FUZZYENTITYPARSER_HPP
#define REDATAMLIB_FUZZYENTITYPARSER_HPP

#include <string>
#include <unordered_map>
#include <utility>  //  std::pair
#include <vector>

#include "ByteArrayReader.hpp"
#include "Entity.hpp"

namespace RedatamLib {
using std::vector;
using std::string;
using std::pair;
using std::unordered_map;

class FuzzyEntityParser {
 public:
  //  throws std::ios_base::failure if fails to open file
  FuzzyEntityParser(const string& filePath);
  ~FuzzyEntityParser() = default;

  FuzzyEntityParser(const FuzzyEntityParser&) = delete;
  FuzzyEntityParser& operator=(const FuzzyEntityParser&) = delete;

  vector<Entity> ParseEntities();

 private:
  ByteArrayReader m_reader;
  string m_rootPath;

  //  throws std::out_of_range
  pair<bool, Entity> TryGetEntity();

  //  Note: changes entities inner values
  static void AssignChildren(vector<Entity>& entitites,
                             unordered_map<string, Entity*> mapping);
};

}  // namespace RedatamLib

#endif  //  REDATAMLIB_FUZZYENTITYPARSER_HPP
