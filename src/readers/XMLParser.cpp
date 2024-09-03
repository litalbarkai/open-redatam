#include <algorithm>    //  std::replace

#include "XMLParser.hpp"
#include "utils.hpp"

namespace RedatamLib
{
using std::cerr, std::endl;

XMLParser::XMLParser()
{
    try
    {
        XMLPlatformUtils::Initialize();

        m_parser = new XercesDOMParser();
        m_parser->setValidationScheme(XercesDOMParser::Val_Never); // No validation
    }
    catch(const XMLException& e)
    {
        cerr << "Error in Xerces initialization: " << TranscodeStr(e.getMessage()) << endl;
    }
}

XMLParser::~XMLParser()
{
    delete m_parser;
    m_parser = nullptr;

    XMLPlatformUtils::Terminate();
}

vector<Entity> XMLParser::ParseFile(const string& fileName)
{
    m_rootPath = FindRootPath(fileName);

    vector<Entity> ret;

    try
    {
        m_parser->parse(fileName.c_str());
        DOMDocument* xmlDoc = m_parser->getDocument();
        DOMElement* rootElement = static_cast<DOMElement*>(xmlDoc->
            getDocumentElement()->
            getElementsByTagName(XMLString::transcode("root"))->item(0));

        DOMElement* child = ParseEntity(&ret, rootElement);
        while (nullptr != child)
        {
            child = ParseEntity(&ret, child);
        }
        
        for (size_t idx = 0; idx < ret.size() - 1; ++idx)
        {
            ret[idx].AttachChild(&ret[idx + 1]);
        }
    }
    catch(const XMLException& e)
    {
        cerr << TranscodeStr(e.getMessage()) << endl;
    }
    catch(const DOMException& e)
    {
        cerr << TranscodeStr(e.getMessage()) << endl;
    }

    return ret;
}

string XMLParser::TranscodeStr(const XMLCh* str)
{
    char* cstr = XMLString::transcode(str);
    string ret = cstr;
    XMLString::release(&cstr);

    return ret;
}

string XMLParser::GetTagValue(DOMElement* element, const string& tag, size_t idx)
{
    DOMNodeList* tags = element->getElementsByTagName(XMLString::transcode(tag.c_str()));

    return (0 != tags->getLength()) ?
        TranscodeStr(static_cast<DOMElement*>(tags->item(idx))->getTextContent()) :
        "";
}

DOMElement* XMLParser::ParseEntity(vector<Entity>* results, DOMElement* e, const string& parentName)
{
    string name = GetTagValue(e, "name");

    string description = GetTagValue(e, "label");
    
    string idxFileName = GetTagValue(e, "filename");
    idxFileName = ReplaceRootPath(m_rootPath, idxFileName);

    Entity curr(name, parentName, description, idxFileName, pair<size_t, size_t>(0, 0));

    shared_ptr<vector<Variable>> variables = ParseVariables(e);
    curr.AttachVariables(variables);

    results->push_back(curr);

    DOMNodeList* children = e->getElementsByTagName(XMLString::transcode("entity"));
    if (0 != children->getLength())
    {
        return static_cast<DOMElement*>(children->item(0));
    }

    return nullptr;
}

shared_ptr<vector<Variable>> XMLParser::ParseVariables(DOMElement* e)
{
    shared_ptr<vector<Variable>> ret(new vector<Variable>);

    DOMNodeList* vars = e->getElementsByTagName(XMLString::transcode("variable"));
    for (size_t idx = 0; idx < vars->getLength(); ++idx)
    {
        DOMElement* var = static_cast<DOMElement*>(vars->item(idx));

        string name = GetTagValue(var, "name");

        pair<VarType, size_t> typeDetails = ParseVarTypeAndSize(var);

        string idxFileName = GetTagValue(var, "filename");
        idxFileName = ReplaceRootPath(m_rootPath, idxFileName);

        string filter = GetTagValue(var, "filter");

        string range = ParseVarRange(var);

        vector<Tag> tags = ParseVarTags(var);

        string description = GetTagValue(var, "label");
        
        string decimalsStr = GetTagValue(var, "decimals");
        size_t decimals = decimalsStr.empty() ? 0 : std::stoi(decimalsStr);

        ret->push_back(Variable(name,
                                typeDetails.first,
                                idxFileName,
                                typeDetails.second,
                                filter,
                                range,
                                tags,
                                description,
                                decimals));
    }

    return ret;
}

pair<VarType, size_t> XMLParser::ParseVarTypeAndSize(DOMElement* var)
{
    DOMElement* details = static_cast<DOMElement*>(
        var->getElementsByTagName(XMLString::transcode("varDicChoice"))->item(0));

    VarType varType = NA;
    string type = GetTagValue(details, "datasetType");
    if ("BIN" == type)
    {
        varType = BIN;
    }
    else if ("CHR" == type)
    {
        varType = CHR;
    }
    else if ("DBL" == type)
    {
        varType = DBL;
    }
    else if ("INT" == type)
    {
        varType = INT;
    }
    else if ("LNG" == type)
    {
        varType = LNG;
    }
    else if ("PCK" == type)
    {
        varType = PCK;
    }

    size_t size = std::stoi(GetTagValue(details, "datasetSize"));

    return {varType, size};
}

string XMLParser::ParseVarRange(DOMElement* var)
{
    DOMNodeList* rangeTags = var->getElementsByTagName(XMLString::transcode("range"));
    if (0 == rangeTags->getLength())
    {
        return "";
    }

    DOMElement* rangeTag = static_cast<DOMElement*>(rangeTags->item(0));

    string ret = GetTagValue(rangeTag, "rangeMin");
    if (!ret.empty())
    {
        ret.append(" TO ");
        ret.append(GetTagValue(rangeTag, "rangeMax"));
    }

    return ret;
}

vector<Tag> XMLParser::ParseVarTags(DOMElement* var)
{
    vector<Tag> ret;

    DOMNodeList* valueLabelTag = var->getElementsByTagName(XMLString::transcode("valueLabels"));
    if (0 == valueLabelTag->getLength())
    {
        return ret;
    }
    DOMNodeList* valueLabels = static_cast<DOMElement*>(valueLabelTag->item(0))
        ->getElementsByTagName(XMLString::transcode("valueLabelElement"));
    
    for (size_t idx = 0; idx < valueLabels->getLength(); ++idx)
    {
        string key = GetTagValue(static_cast<DOMElement*>(valueLabels->item(idx)),
                                "value");
        string value = GetTagValue(static_cast<DOMElement*>(valueLabels->item(idx)),
                                "label");
        
        ret.push_back(Tag(key, value));
    }

    ret.push_back(Tag(GetTagValue(var, "missing"), "MISSING"));
    ret.push_back(Tag(GetTagValue(var, "notappl"), "NOTAPPLICABLE"));

    return ret;
}
}   // namespace RedatamLib