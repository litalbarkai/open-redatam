#include "Variable.hpp"

namespace RedatamLib
{
Variable::Variable() :
    m_name(""),
    m_type(NA),
    m_idxFileName(""),
    m_dataSize(0),
    m_filter(""),
    m_range(""),
    m_tags(),
    m_description(""),
    m_decimals(0)
{}

Variable::Variable(string name,
                    VarType type,
                    string idxFileName,
                    size_t dataSize,
                    string filter,
                    string range,
                    vector<Tag> tags,
                    string description,
                    size_t decimals) :
    m_name(name),
    m_type(type),
    m_idxFileName(idxFileName),
    m_dataSize(dataSize),
    m_filter(filter),
    m_range(range),
    m_tags(tags),
    m_description(description),
    m_decimals(decimals)
{
    ParseValues();
}

void Variable::ParseValues()
{
    ByteArrayReader reader(m_idxFileName);

    switch (m_type)
    {
    case BIN:
        break;

    case CHR:
        ParseStrings(m_dataSize, reader);
        break;
        
    case DBL:
        break;
        
    case INT:
        break;
        
    case LNG:
        break;
        
    case PCK:
        break;
    }
}

void Variable::ParseStrings(size_t length, ByteArrayReader reader)
{
    vector<string>* vals = new vector<string>();

    try
    {
        while (true)
        {
            vals->push_back(reader.ReadString(length));
        }
    }
    catch(const std::out_of_range&)
    {}

    m_values = static_cast<void*>(vals);
}

} // namespace RedatamLib