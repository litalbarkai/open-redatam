#ifndef REDATAMLIB_XMLPARSER_HPP
#define REDATAMLIB_XMLPARSER_HPP

#include "Entity.hpp"
#include "pugixml.hpp"

#include <iostream>

namespace RedatamLib {
using std::pair;
using std::shared_ptr;
using std::string;
using std::vector;

class XMLParser {
public:
  XMLParser() = default;
  ~XMLParser() = default;

  XMLParser(const XMLParser &) = delete;
  XMLParser &operator=(const XMLParser &) = delete;

  vector<Entity> ParseFile(const string &fileName);

private:
  string m_rootPath;
  string GetTagValue(pugi::xml_node node, const string &tag, size_t idx = 0);
  pugi::xml_node ParseEntity(vector<Entity> *results, pugi::xml_node node,
                             const string &parentName = "");
  shared_ptr<vector<Variable>> ParseVariables(pugi::xml_node node);
  pair<VarType, size_t> ParseVarTypeAndSize(pugi::xml_node var);
  string ParseVarRange(pugi::xml_node var);
  vector<Tag> ParseVarTags(pugi::xml_node var);
};
} // namespace RedatamLib

#endif //  REDATAMLIB_XMLPARSER_HPP
