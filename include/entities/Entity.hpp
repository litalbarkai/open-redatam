#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <vector>
#include <utility>  //  std::pair

#include "Variable.hpp"

namespace RedatamLib
{
using std::string, std::vector, std::pair;

class Entity {
public:
    explicit Entity();
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

private:
    string m_name;
    string m_description;
    string m_indexFilename;
    string m_parentName;
    Entity* m_child;
    vector<Variable> m_variables;
    pair<size_t, size_t> m_bounds;
};
} // namespace RedatamLib

#endif // ENTITY_HPP
