#include "FuzzyEntityParser.hpp"
#include "FuzzyVariableParser.hpp"
#include "utils/utils.hpp"  //  GetFileExtension, ThrowIfBad

namespace RedatamLib {

using std::make_pair;
using std::move;

FuzzyEntityParser::FuzzyEntityParser(const string &filePath)
    : m_reader(filePath), m_rootPath(FindRootPath(filePath)) {}

vector<Entity> FuzzyEntityParser::ParseEntities() {
  std::pair<bool, Entity> curr;
  vector<Entity> ret;
  unordered_map<string, Entity *> entities;

  try {
    while (true) {
      curr = TryGetEntity();
      if (curr.first) {
        ret.push_back(move(curr.second));
      } else {
        m_reader.MovePos(1);
      }
    }
  } catch (const std::out_of_range &) {
  }

  if (ret.empty()) {
    throw std::out_of_range("Error: No entities found.");
  }

  for (Entity &e : ret) {
    entities[e.GetName()] = &e;
  }

  AssignChildren(ret, entities);

  FuzzyVariableParser varParser(m_reader, m_rootPath);
  varParser.ParseAllVariables(ret);

  return ret;
}

std::pair<bool, Entity> FuzzyEntityParser::TryGetEntity() {
  size_t originalPosition = m_reader.GetPos();

  string entityName;
  if (!m_reader.TryReadStr(&entityName) || entityName.empty()) {
    m_reader.SetPos(originalPosition);
    return {false, Entity()};
  }

  string entityNameRepeated;
  if (!m_reader.TryReadStr(&entityNameRepeated)) {
    m_reader.MovePos(2);
  } else if (entityName != entityNameRepeated) {
    m_reader.SetPos(originalPosition);
    return {false, Entity()};
  }

  string parentEntityName;
  if (!entityNameRepeated.empty() && !m_reader.TryReadStr(&parentEntityName)) {
  }

  string description;
  if (!m_reader.TryReadStr(&description, false)) {
    m_reader.MovePos(2);
  }

  string idxFileName, ext;
  if ((!m_reader.TryReadStr(&idxFileName, false)) ||
      !(!idxFileName.empty() && TryGetFileExtension(idxFileName, &ext) &&
        ".ptr" == ext)) {
    m_reader.SetPos(originalPosition);
    return {false, Entity()};
  }
  idxFileName = ReplaceRootPath(m_rootPath, idxFileName);

  std::pair<size_t, size_t> bounds =
      make_pair(originalPosition, m_reader.GetPos());

  return {true, Entity(entityName, parentEntityName, description, idxFileName,
                       bounds)};
}

//  static
void FuzzyEntityParser::AssignChildren(
    vector<Entity> &entitites, unordered_map<string, Entity *> mapping) {
  for (Entity &e : entitites) {
    string parent = e.GetParentName();
    if (!parent.empty()) {
      mapping[parent]->AttachChild(&e);
    }
  }
}

}  // namespace RedatamLib
