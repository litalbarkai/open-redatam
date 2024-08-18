#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <vector>
#include <cstddef>  //  std::byte
#include <utility>  //  std::pair

#include "Variable.hpp"

namespace RedatamLib
{
using std::string, std::vector, std::byte, std::pair;

class Entity {
public:
    explicit Entity();   //open pointer?
    explicit Entity(string name,
            string parentName,
            string description,
            string idxFileName,
            pair<size_t, size_t> bounds);
    ~Entity() = default;

    string GetName() const;
    string GetParentName() const;

    pair<size_t, size_t> GetBounds() const;

    void AttachChild(Entity* child);
    void AttachVariables(vector<Variable> variables);

    // int GetPointerData();

    // long GetRowsCount();

    // vector<Variable> GetSelectedVariables();

private:
    // byte m_b1;
    // int m_i1;
    // int m_s1;
    // int m_level;
    // int m_variableCount;
    // long m_rowsCount;
    // string m_c1;
    string m_name;
    // string m_alias;
    // string m_rootPath;
    string m_description;
    string m_indexFilename;
    string m_parentName;
    // string m_codesVariable;
    // string m_labelVariable;
    vector<Entity*> m_children;
    vector<Variable> m_variables;
    pair<size_t, size_t> m_bounds;
};
} // namespace RedatamLib

#endif // ENTITY_HPP
