#include <filesystem>
#include <sstream>          //  std::ostringstream
#include <fstream>          //  std::ofstream
#include <iostream>         //  std::endl
#include <stdexcept>        //  std::runtime_error

#include <locale>
#include <codecvt>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>
#include <xercesc/dom/DOMLSOutput.hpp>

#include "XMLExporter.hpp"
#include "XMLUtils.hpp"
#include "utils.hpp"

namespace RedatamLib
{
using std::ostringstream, std::ofstream, std::endl, std::runtime_error;
namespace fs = std::filesystem;
XERCES_CPP_NAMESPACE_USE

XMLExporter::XMLExporter(const string& outputDirectory) :
    m_path(outputDirectory + "/redatam_converter_description.xml"),
    m_writer(nullptr),
    m_doc(nullptr)
{
    try
    {
        XMLPlatformUtils::Initialize();

        m_writer = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode("Core"));
        ThrowIfBad(nullptr != m_writer, runtime_error("failed to create DOMImplementation."));

        m_doc = m_writer->createDocument(nullptr, XMLString::transcode("database"), nullptr);
    }
    catch (const XMLException& e)
    {
        cerr << "Error in Xerces initialization: " << TranscodeStr(e.getMessage()) << endl;
        throw;
    }
    catch (const runtime_error& e)
    {
        cerr << "Error in Xerces initialization: " << e.what() << endl;
        throw;
    }
}

XMLExporter::~XMLExporter()
{
    m_doc->release();
    m_writer = nullptr;

    XMLPlatformUtils::Terminate();
}


void XMLExporter::ExportSummary(vector<Entity>& entities)
{
    DOMElement* root = m_doc->getDocumentElement();

    CreateEntityElement(entities[0], root);

    DOMLSSerializer* serializer = ((DOMImplementationLS*)m_writer)->createLSSerializer();
    DOMLSOutput* output = ((DOMImplementationLS*)m_writer)->createLSOutput();

    XMLFormatTarget* formatTarget = new LocalFileFormatTarget(m_path.c_str());
    output->setByteStream(formatTarget);

    if (serializer->getDomConfig()->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
    {
        serializer->getDomConfig()->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
    }
    
    serializer->write(m_doc, output);

    delete formatTarget;
    output->release();
    serializer->release();
}

void XMLExporter::SetAttribute(DOMElement* e, const string& name, const string& value)
{
    try
    {
        e->setAttribute(XMLString::transcode(name.c_str()),
                        XMLString::transcode(value.c_str()));
    }
    catch(const XMLException& err)
    {
        e->setAttribute(TranscodeBinary(name), TranscodeBinary(value));
    }
}

void XMLExporter::CreateEntityElement(Entity& e, DOMElement* parentTag)
{
    DOMElement* elem = m_doc->createElement(XMLString::transcode("entity"));
    
    SetAttribute(elem, "name", e.GetName());
    SetAttribute(elem, "pointer", e.GetPTRPath());

    for (Variable& v : *(e.GetVariables().get()))
    {
        CreateVariableElement(v, elem);
    }

    Entity* child = e.GetChild(); 
    if (nullptr != child)
    {
        CreateEntityElement(*child, elem);
    }

    parentTag->appendChild(elem);
}

void XMLExporter::CreateVariableElement(Variable& v, DOMElement* parentTag)
{
    DOMElement* elem = m_doc->createElement(XMLString::transcode("variable"));
    
    SetAttribute(elem, "name", v.GetName());
    SetAttribute(elem, "label", v.GetDescription());
    SetAttribute(elem, "type", GetVarType(v.GetType()));
    SetAttribute(elem, "size", std::to_string(v.GetDataSize()));
    SetAttribute(elem, "decimals", std::to_string(v.GetDecimals()));
    SetAttribute(elem, "range", v.GetRange());
    SetAttribute(elem, "filter", v.GetFilter());
    SetAttribute(elem, "data", v.GetFilePath());

    vector<Tag> tags = v.GetTags();
    if (!tags.empty())
    {
        DOMElement* tagsHeader = m_doc->createElement(XMLString::transcode("valueLabels"));

        for (Tag& t : tags)
        {
            DOMElement* label = m_doc->createElement(XMLString::transcode("valueLabel"));
            SetAttribute(label, "name", t.first);
            SetAttribute(label, "value", t.second);

            tagsHeader->appendChild(label);
        }

        elem->appendChild(tagsHeader);
    }

    parentTag->appendChild(elem);
}

//  static
string XMLExporter::GetVarType(VarType type)
{
    switch (type)
    {
    case CHR:
        return "STRING";

    case DBL:
        return "REAL";

    case INT:
        return "INT16";

    case LNG:
    case BIN:
    case PCK:
        return "INTEGER";

    case NA:
    default:
        return "";
    }
}

//  static
XMLCh* XMLExporter::TranscodeBinary(const string& str)
{
    string ret;
    std::wstring wide_string;

    for (unsigned char c : str)
    {
        wide_string.push_back(c);
    }

    // Convert the wstring to a UTF-8 string
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    ret = converter.to_bytes(wide_string);

    return XMLString::transcode(ret.c_str());
}
} // namespace RedatamLib