#ifndef REDATAMLIB_XMLPARSER_HPP
#define REDATAMLIB_XMLPARSER_HPP

#include <iostream>
#include <memory>  //  std::shared_ptr
#include <string>
#include <utility>  //  std::pair
#include <vector>

#include "Entity.hpp"
#include "Variable.hpp"
#include "pugixml.hpp"

namespace RedatamLib {
using std::string;
using std::vector;
using std::shared_ptr;
using std::pair;

class XMLParser {
 public:
  XMLParser() = default;
  ~XMLParser() = default;

  XMLParser(const XMLParser&) = delete;
  XMLParser& operator=(const XMLParser&) = delete;

  vector<Entity> ParseFile(const string& fileName);

 private:
  string m_rootPath;

  string GetTagValue(pugi::xml_node node, const string& tag, size_t idx = 0);
  //  returns the next <entity>
  pugi::xml_node ParseEntity(vector<Entity>* results, pugi::xml_node node,
                             const string& parentName = "");
  shared_ptr<vector<Variable>> ParseVariables(pugi::xml_node node);
  pair<VarType, size_t> ParseVarTypeAndSize(pugi::xml_node var);
  string ParseVarRange(pugi::xml_node var);
  vector<Tag> ParseVarTags(pugi::xml_node var);
};
}  // namespace RedatamLib

#endif  //  REDATAMLIB_XMLPARSER_HPP
