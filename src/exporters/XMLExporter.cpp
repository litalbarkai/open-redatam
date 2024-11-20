#include "XMLExporter.hpp"
#include "utils.hpp"

#include <fstream>   // ofstream
#include <iostream>  // endl
#include <sstream>   // ostringstream
#include <stdexcept> // runtime_error

namespace RedatamLib {
using std::endl;
using std::ofstream;
using std::ostringstream;
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

void XMLExporter::SetAttribute(pugi::xml_node &node, const string &name,
                               const string &value) {
  node.append_attribute(name.c_str()) = value.c_str();
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
