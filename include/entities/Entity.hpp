#ifndef REDATAMLIB_ENTITY_HPP
#define REDATAMLIB_ENTITY_HPP

#include <string>
#include <vector>
#include <utility>  //  std::pair

#include "Variable.hpp"
#include "ByteArrayReader.hpp"

namespace RedatamLib
{
using std::string, std::vector, std::pair;

class Entity {
public:
    explicit Entity();
    explicit Entity(const string& name,
            const string& parentName,
            const string& description,
            const string& idxFileName,
            pair<size_t, size_t> bounds);
    ~Entity() = default;

    string GetName() const;
    string GetParentName() const;
    vector<Variable> GetVariables() const;
    size_t GetRowsCount() const;

    pair<size_t, size_t> GetBounds() const;

    size_t GetPTRData();

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
    ByteArrayReader m_reader;
    size_t m_rowsCount;
};
} // namespace RedatamLib

#endif // REDATAMLIB_ENTITY_HPP
