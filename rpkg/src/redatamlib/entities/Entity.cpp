#include "Entity.hpp"

namespace RedatamLib {

Entity::Entity()
    : m_name(""),
      m_parentName(""),
      m_description(""),
      m_indexFilename(""),
      m_child(nullptr),
      m_variables(),
      m_bounds({0, 0}),
      m_reader(),
      m_rowsCount(0) {}

Entity::Entity(const string &name, const string &parentName,
               const string &description, const string &idxFileName,
               pair<size_t, size_t> bounds)
    : m_name(name),
      m_parentName(parentName),
      m_description(description),
      m_indexFilename(idxFileName),
      m_child(nullptr),
      m_variables(),
      m_bounds(bounds),
      m_reader(idxFileName),
      m_rowsCount(0) {
  // PTR files shouldn't be empty, but in the odd case it is - treat it as
  // holding zeros
  if (m_reader.GetEndPos() == m_reader.GetPos()) {
    m_rowsCount = 0;
  } else {
    m_reader.SetPos(m_reader.GetEndPos() - 4);
    m_rowsCount = m_reader.ReadInt32LE();

    m_reader.SetPos(4);  // advance past header of zeros
  }
}

string Entity::GetName() const { return m_name; }

string Entity::GetParentName() const { return m_parentName; }

string Entity::GetPTRPath() const { return m_indexFilename; }

shared_ptr<vector<Variable>> Entity::GetVariables() const {
  return m_variables;
}

size_t Entity::GetRowsCount() const { return m_rowsCount; }

pair<size_t, size_t> Entity::GetBounds() const { return m_bounds; }

size_t Entity::GetPTRData() { return m_reader.ReadInt32LE(); }

Entity *Entity::GetChild() const { return m_child; }

void Entity::AttachChild(Entity *child) { m_child = child; }

void Entity::AttachVariables(shared_ptr<vector<Variable>> variables) {
  m_variables = variables;
}

}  // namespace RedatamLib
