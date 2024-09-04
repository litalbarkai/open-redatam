#ifndef REDATAMLIB_XMLPARSER_HPP
#define REDATAMLIB_XMLPARSER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>           //  std::shared_ptr
#include <utility>          //  std::pair

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLString.hpp>

#include "Entity.hpp"
#include "Variable.hpp"

namespace RedatamLib
{
using namespace XERCES_CPP_NAMESPACE;
using std::string, std::vector, std::shared_ptr, std::pair;

class XMLParser
{
public:
    explicit XMLParser();
    ~XMLParser();

    XMLParser(const XMLParser&) = delete;
    XMLParser& operator=(const XMLParser&) = delete;

    vector<Entity> ParseFile(const string& fileName);

private:
    XercesDOMParser* m_parser;
    string m_rootPath;

    string TranscodeStr(const XMLCh* str);
    string GetTagValue(DOMElement* element, const string& tag, size_t idx = 0);
    //  returns the next <entity> DOMElement*
    DOMElement* ParseEntity(vector<Entity>* results,
                        DOMElement* element,
                        const string& parentName = "");
    shared_ptr<vector<Variable>> ParseVariables(DOMElement* e);
    vector<DOMElement*> GetChildren(DOMElement* parent, const string& tag);
    pair<VarType, size_t> ParseVarTypeAndSize(DOMElement* var);
    string ParseVarRange(DOMElement* var);
    vector<Tag> ParseVarTags(DOMElement* var);
};
} // namespace RedatamLib

#endif  //  REDATAMLIB_XMLPARSER_HPP