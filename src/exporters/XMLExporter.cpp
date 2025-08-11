#include "XMLExporter.hpp"
#include "utils.hpp"

#include <algorithm> // replace
#include <fstream>   // ofstream
#include <iostream>  // endl
#include <sstream>   // ostringstream
#include <stdexcept> // runtime_error

namespace RedatamLib {
using std::endl;
using std::ofstream;
using std::ostringstream;
using std::replace;
using std::runtime_error;

XMLExporter::XMLExporter(const std::string &outputDirectory)
    : m_path(outputDirectory + "/redatam_converter_description.xml") {
  pugi::xml_node decl = m_doc.append_child(pugi::node_declaration);
  decl.append_attribute("version") = "1.0";
  decl.append_attribute("encoding") = "UTF-8";
}

void XMLExporter::ExportSummary(vector<Entity> &entities) {
  pugi::xml_node root = m_doc.append_child("RedatamConverterDescription");

  for (Entity &e : entities) {
    CreateEntityElement(e, root);
  }

  ofstream fs(m_path);
  if (!fs.is_open()) {
    throw runtime_error("Error: Failed to create file.");
  }
  m_doc.save(fs);
}

string XMLExporter::CleanString(const string &input) {
  string output;
  for (char c : input) {
    if (c == '\0') {
      // Replace null bytes with spaces
      output += ' ';
    } else if ((c & 0x80) == 0) {
      // ASCII character
      output += c;
    } else {
      // Replace non-UTF-8 character with a placeholder
      output += '?';
    }
  }
  return output;
}

void XMLExporter::SetAttribute(pugi::xml_node &node, const string &name,
                               const string &value) {
  string cleanValue = CleanString(value);
  node.append_attribute(name.c_str()) = cleanValue.c_str();
}

void XMLExporter::CreateEntityElement(Entity &e, pugi::xml_node &parentTag) {
  pugi::xml_node entityNode = parentTag.append_child("Entity");
  SetAttribute(entityNode, "name", e.GetName());

  for (Variable &v : *(e.GetVariables().get())) {
    CreateVariableElement(v, entityNode);
  }
}

void XMLExporter::CreateVariableElement(Variable &v,
                                        pugi::xml_node &parentTag) {
  pugi::xml_node variableNode = parentTag.append_child("Variable");
  SetAttribute(variableNode, "name", v.GetName());
  SetAttribute(variableNode, "type", GetVarType(v.GetType()));
  SetAttribute(variableNode, "description", v.GetDescription());

  if (!v.GetTags().empty()) {
    pugi::xml_node tagsNode = variableNode.append_child("Tags");
    for (Tag &t : v.GetTags()) {
      pugi::xml_node tagNode = tagsNode.append_child("Tag");
      SetAttribute(tagNode, "key", t.first);
      SetAttribute(tagNode, "value", t.second);
    }
  }
}

string XMLExporter::GetVarType(VarType type) {
  switch (type) {
  case BIN:
    return "BIN";
  case PCK:
    return "PCK";
  case INT:
    return "INT";
  case LNG:
    return "LNG";
  case CHR:
    return "CHR";
  case DBL:
    return "DBL";
  default:
    return "UNKNOWN";
  }
}
} // namespace RedatamLib
