#include "Entity.hpp"

namespace RedatamLib
{
using std::string, std::vector, std::byte;

Entity::Entity()
{}

Entity::Entity(string name, string parentName, string description, string idxFileName) :
    m_name(name),
    m_parentName(parentName),
    m_description(description),
    m_indexFilename(idxFileName)
{}

string Entity::GetName() const
{
    return m_name;
}

string Entity::GetParentName() const
{
    return m_parentName;
}

void Entity::AttachChild(Entity* child)
{
    m_children.push_back(child);
}

} // namespace RedatamLib
