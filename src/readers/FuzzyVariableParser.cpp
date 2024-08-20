#include <algorithm>    //  std::replace

#include "FuzzyVariableParser.hpp"
#include "utils.hpp"                //  GetFileExtension

namespace RedatamLib
{
FuzzyVariableParser::FuzzyVariableParser(string filePath) : m_reader(filePath)
{}

FuzzyVariableParser::FuzzyVariableParser(ByteArrayReader reader) : m_reader(reader)
{}

void FuzzyVariableParser::ParseAllVariables(vector<Entity>& entities)
{
    vector<pair<size_t, size_t>> searchBounds = GetSearchBounds(entities);

    for (size_t i = 0; i < entities.size(); ++i)
    {
        vector<Variable> vars;
        ParseVariables(&vars, searchBounds[i]);
        entities[i].AttachVariables(vars);
    }
}

vector<pair<size_t, size_t>> FuzzyVariableParser::GetSearchBounds(vector<Entity> entities)
{
    vector<pair<size_t, size_t>> ret;

    for (size_t i = 0; i < entities.size() - 1; ++i)
    {
        ret.push_back({entities[i].GetBounds().second,
                        entities[i+1].GetBounds().first});
    }
    ret.push_back({entities.back().GetBounds().second,
                    m_reader.GetEndPos()});

    return ret;
}

void FuzzyVariableParser::ParseVariables(vector<Variable>* output,
                                        pair<size_t, size_t> bounds)
{
    try
    {
        m_reader.SetPos(bounds.first);

        while (true)
        {
            m_reader.MovePosTo("DATASET");
            ThrowIfBad(m_reader.GetPos() < bounds.second,
                std::out_of_range("Error: DATASET doesn't belong to current entity."));

            m_reader.MovePos(-2);    //  "DATASET" length indicator 
            string varName = m_reader.GetFormerString();

            m_reader.MovePos(10);    //  "DATASET" + length indicator + " "
            VarType type = ParseType();
            string idxFileName = ParseIdxFileName();
            size_t dataSize = ParseDataSize(type);
            string filter;
            if (!m_reader.TryReadShortStr(&filter))
            {
                m_reader.MovePos(2);
            }
            string range;
            if (!m_reader.TryReadShortStr(&range))
            {
                m_reader.MovePos(2);
            }
            vector<Tag> tags = ParseTags();
            string description;
            if (!m_reader.TryReadShortStr(&description))
            {
                m_reader.MovePos(2);
            }
            ParseMissingAndNA(&tags);
            size_t decimals = 0;
            if (DBL == type)
            {
                decimals = ParseDecimals();
            }
            
            output->push_back(Variable(varName,
                                        type,
                                        idxFileName,
                                        dataSize,
                                        filter,
                                        range,
                                        tags,
                                        description,
                                        decimals));
        }
    }
    catch (const std::out_of_range&)
    {}
}

VarType FuzzyVariableParser::ParseType()
{
    string type = m_reader.ReadString(3);
    m_reader.MovePos(1);

    if ("BIN" == type)
    {
        return BIN;
    }
    else if ("CHR" == type)
    {
        return CHR;
    }
    else if ("DBL" == type)
    {
        return DBL;
    }
    else if ("INT" == type)
    {
        return INT;
    }
    else if ("LNG" == type)
    {
        return LNG;
    }
    else if ("PCK" == type)
    {
        return PCK;
    }
    
    return NA;
}

string FuzzyVariableParser::ParseIdxFileName()
{
    size_t ogPos = m_reader.GetPos();

    m_reader.MovePos(1);
    m_reader.MovePosTo("'");
    size_t len = m_reader.GetPos() - ogPos - 1;

    m_reader.SetPos(ogPos + 1);
    string ret = m_reader.ReadString(len);
    m_reader.MovePos(1);    //  "'"

    std::replace(ret.begin(), ret.end(), '\\', '/');

    //DEBUG
    ret.replace(0, 40, "/home/little/Downloads/censochile/input");

    return ret;
}

size_t FuzzyVariableParser::ParseDataSize(VarType type)
{
    if (DBL == type)
    {
        return 8;
    }

    m_reader.MovePos(6);    //  " SIZE "
    size_t len = GetSubstringLength("");
    return std::stoi(m_reader.ReadString(len));
}

vector<Tag> FuzzyVariableParser::ParseTags()
{
    vector<Tag> ret;

    string discard("");
    m_reader.TryReadShortStr(&discard); //  data type identifier ("INTEGER", "STRING", "REAL")

    size_t ogPos = m_reader.GetPos();
    size_t len = m_reader.ReadInt16LE();
    while (m_reader.GetPos() - ogPos < len + 2)
    {
        size_t keyLen = GetSubstringLength(" ");
        string key = m_reader.ReadString(keyLen);

        m_reader.MovePos(1);    //  " "

        size_t valLen = GetSubstringLength("\t");
        string value = m_reader.ReadString(valLen);

        ret.push_back(Tag(key, value));

        m_reader.MovePos(1);    //  "\t"
    }

    return ret;
}

void FuzzyVariableParser::ParseMissingAndNA(vector<Tag>* tags)
{
    string val1 = "MISSING";
    string val2 = "NOTAPPLICABLE";

    m_reader.MovePosTo(val1);
    m_reader.MovePos(val1.size() + 1);  //  val1 + " "
    size_t keyLen1 = GetSubstringLength(" ");
    string key1 = m_reader.ReadString(keyLen1);
    tags->push_back(Tag(key1, val1));

    m_reader.MovePos(1 + val2.size() + 1);  //  " " + val2 + " "
    size_t keyLen2 = std::min(GetSubstringLength(""), GetSubstringLength(" "));
    string key2 = m_reader.ReadString(keyLen2);
    tags->push_back(Tag(key2, val2));
}

size_t FuzzyVariableParser::ParseDecimals()
{
    m_reader.MovePos(10);   //  " DECIMALS "
    size_t len = std::min(GetSubstringLength(""), GetSubstringLength(" "));
    return std::stoi(m_reader.ReadString(len));
}

size_t FuzzyVariableParser::GetSubstringLength(string delimiter)
{
    if (delimiter.empty())
    {
        delimiter = '\0';
    }

    size_t ogPos = m_reader.GetPos();
    m_reader.MovePosTo(delimiter);
    size_t ret = m_reader.GetPos() - ogPos;
    m_reader.SetPos(ogPos);

    return ret;
}

} // namespace RedatamLib