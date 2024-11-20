#include "XMLParser.hpp"
#include "utils.hpp"

#include <algorithm>          // replace
#include <cpp11/function.hpp> // message

namespace RedatamLib {

using cpp11::message;
using pugi::xml_node;
using std::exception;
using std::make_shared;
using std::move;
using std::runtime_error;
using std::stoi;
using std::string;

vector<Entity> XMLParser::ParseFile(const string &fileName) {
  m_rootPath = FindRootPath(fileName);

  vector<Entity> ret;

  try {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fileName.c_str());
    if (!result) {
      string errorMsg = "Error parsing file: " + string(result.description());
      throw runtime_error(errorMsg);
    }

    // Look for the correct root element
    xml_node redDictionaryNode = doc.child("redDictionary_XML");
    if (!redDictionaryNode) {
      throw runtime_error("redDictionary_XML element not found");
    }

    xml_node rootElement = redDictionaryNode.child("root");
    if (!rootElement) {
      throw runtime_error("Root element not found");
    }

    xml_node child = ParseEntity(&ret, rootElement);
    while (child) {
      child = ParseEntity(&ret, child, ret.back().GetName());
    }

    for (size_t idx = 0; idx < ret.size() - 1; ++idx) {
      ret[idx].AttachChild(&ret[idx + 1]);
    }
  } catch (const exception &e) {
    string errorMsg = "Error: " + string(e.what());
    message(errorMsg.c_str());
    throw;
  }

  return ret;
}

string XMLParser::GetTagValue(xml_node node, const string &tag, size_t idx) {
  xml_node child = node.child(tag.c_str());
  return child ? child.child_value() : "";
}

xml_node XMLParser::ParseEntity(vector<Entity> *results, xml_node node,
                                const string &parentName) {
  string name = GetTagValue(node, "name");

  string description = GetTagValue(node, "label");

  string idxFileName = GetTagValue(node, "filename");
  idxFileName = ReplaceRootPath(m_rootPath, idxFileName);

  Entity curr(name, parentName, description, idxFileName,
              pair<size_t, size_t>(0, 0));

  shared_ptr<vector<Variable>> variables = ParseVariables(node);
  curr.AttachVariables(variables);

  results->push_back(std::move(curr));

  xml_node child = node.child("entity");
  return child;
}

shared_ptr<vector<Variable>> XMLParser::ParseVariables(xml_node node) {
  shared_ptr<vector<Variable>> ret = make_shared<vector<Variable>>();

  for (xml_node var : node.children("variable")) {
    string name = GetTagValue(var, "name");

    pair<VarType, size_t> typeDetails = ParseVarTypeAndSize(var);

    string idxFileName = GetTagValue(var, "filename");
    idxFileName = ReplaceRootPath(m_rootPath, idxFileName);

    string filter = GetTagValue(var, "filter");

    string range = ParseVarRange(var);

    vector<Tag> tags = ParseVarTags(var);

    string description = GetTagValue(var, "label");

    string decimalsStr = GetTagValue(var, "decimals");
    size_t decimals = decimalsStr.empty() ? 0 : stoi(decimalsStr);

    ret->emplace_back(name, typeDetails.first, idxFileName, typeDetails.second,
                      filter, range, tags, description, decimals);
  }

  return ret;
}

pair<VarType, size_t> XMLParser::ParseVarTypeAndSize(xml_node var) {
  xml_node details = var.child("varDicChoice");

  VarType varType = NA;
  string type = GetTagValue(details, "datasetType");
  if ("BIN" == type) {
    varType = BIN;
  } else if ("CHR" == type) {
    varType = CHR;
  } else if ("DBL" == type) {
    varType = DBL;
  } else if ("INT" == type) {
    varType = INT;
  } else if ("LNG" == type) {
    varType = LNG;
  } else if ("PCK" == type) {
    varType = PCK;
  }

  size_t size = stoi(GetTagValue(details, "datasetSize"));

  return {varType, size};
}

string XMLParser::ParseVarRange(xml_node var) {
  xml_node rangeTag = var.child("range");
  if (!rangeTag) {
    return "";
  }

  string ret = GetTagValue(rangeTag, "rangeMin");
  if (!ret.empty()) {
    ret.append(" TO ");
    ret.append(GetTagValue(rangeTag, "rangeMax"));
  }

  return ret;
}

vector<Tag> XMLParser::ParseVarTags(xml_node var) {
  vector<Tag> ret;

  xml_node valueLabelTag = var.child("valueLabels");
  if (!valueLabelTag) {
    return ret;
  }

  for (xml_node valueLabel : valueLabelTag.children("valueLabelElement")) {
    string key = GetTagValue(valueLabel, "value");
    string value = GetTagValue(valueLabel, "label");

    ret.emplace_back(key, value);
  }

  ret.emplace_back(GetTagValue(var, "missing"), "MISSING");
  ret.emplace_back(GetTagValue(var, "notappl"), "NOTAPPLICABLE");

  return ret;
}

} // namespace RedatamLib
