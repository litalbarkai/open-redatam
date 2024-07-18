#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <vector>
#include <cstddef>  //  std::byte

#include "Variable.hpp"

namespace RedatamLib {
using std::string, std::vector, std::byte;

class Entity {
public:
    Entity();   //open pointer?
    ~Entity();

    // int GetPointerData();

    // long GetRowsCount();

    // vector<Variable> GetSelectedVariables();

private:
    byte m_b1;
    int m_i1;
    int m_s1;
    int m_level;
    int m_variableCount;
    long m_rowsCount;
    string m_c1;
    string m_name;
    string m_alias;
    string m_rootPath;
    string m_description;
    string m_indexFilename;
    string m_relationChild;
    string m_relationParent;
    string m_codesVariable;
    string m_labelVariable;
    vector<Entity> m_children;
    vector<Variable> m_variables; //sharedptr or references?
};
} // namespace RedatamLib

#endif // ENTITY_HPP
