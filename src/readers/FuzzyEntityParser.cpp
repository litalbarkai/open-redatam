#include "FuzzyEntityParser.hpp"
#include "FuzzyVariableParser.hpp"
#include "utils.hpp"                //  GetFileExtension, ThrowIfBad

namespace RedatamLib
{
FuzzyEntityParser::FuzzyEntityParser(string filePath) : m_reader(filePath)
{}

FuzzyEntityParser::FuzzyEntityParser(ByteArrayReader reader) : m_reader(reader)
{}

vector<Entity> FuzzyEntityParser::ParseEntities()
{
    pair<bool, Entity> curr;
    vector<Entity> ret;
    unordered_map<string, Entity*> entities;

    try
    {
        while (true)
        {
            curr = TryGetEntity();
            if (curr.first)
            {
                ret.push_back(curr.second);
            }
            else
            {
                m_reader.MovePos(1);
            }
        }
    }
    catch (const std::out_of_range&)
    {}

    for (Entity& e : ret)
    {
        entities[e.GetName()] = &e;
    }

    AssignChildren(ret, entities);

    FuzzyVariableParser varParser(m_reader);
    varParser.ParseAllVariables(ret);

    return ret;
}

pair<bool, Entity> FuzzyEntityParser::TryGetEntity()
{
    size_t ogPos = m_reader.GetPos();

    string entityName("");
    if (!m_reader.TryReadShortStr(&entityName) || entityName.empty())
    {
        m_reader.SetPos(ogPos);
        return pair(false, Entity());
    }

    string entityNameRepeated("");
    if (!m_reader.TryReadShortStr(&entityNameRepeated))
    {
        m_reader.MovePos(2);
    }
    else if (entityName != entityNameRepeated)
    {
        m_reader.SetPos(ogPos);
        return pair(false, Entity());
    }

    string parentEntityName("");
    if (!entityNameRepeated.empty() && !m_reader.TryReadShortStr(&parentEntityName))
    {
        // m_reader.SetPos(ogPos);
        // return pair(false, Entity());
    }

    string description("");
    if (!m_reader.TryReadShortStr(&description))
    {
        // m_reader.SetPos(ogPos);
        // return pair(false, Entity());
        m_reader.MovePos(2);
    }

    string idxFileName(""), ext("");
    if ((!m_reader.TryReadShortStr(&idxFileName, false)) ||
        !(!idxFileName.empty() && TryGetFileExtension(idxFileName, &ext) &&
        ".ptr" == ext))
    {
        m_reader.SetPos(ogPos);
        return pair(false, Entity());
    }

    pair<size_t, size_t> bounds(ogPos, m_reader.GetPos());

    return pair(true,
            Entity(entityName, parentEntityName, description, idxFileName, bounds));
}

//  static
void FuzzyEntityParser::AssignChildren(vector<Entity>& entitites,
                                        unordered_map<string, Entity*> mapping)
{
    for (Entity& e : entitites)
    {
        string parent = e.GetParentName();
        if ("" != parent)
        {
            mapping[parent]->AttachChild(&e);
        }
    }
}

} // namespace RedatamLib
