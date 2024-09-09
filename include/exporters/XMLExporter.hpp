#ifndef REDATAMLIB_XMLEXPORTER_HPP
#define REDATAMLIB_XMLEXPORTER_HPP

#include "Entity.hpp"

namespace RedatamLib
{
using std::string;
using namespace XERCES_CPP_NAMESPACE;

class XMLExporter {
public:
    explicit XMLExporter(const string& outputDirectory);
    ~XMLExporter();

    XMLExporter(const XMLExporter&) = delete;
    XMLExporter& operator=(const XMLExporter&) = delete;

    void ExportSummary(vector<Entity>& entities);

private:
    string m_path;
    DOMImplementation* m_writer;
    DOMDocument* m_doc;

    void SetAttribute(DOMElement* e, const string& name, const string& value);
    void CreateEntityElement(Entity& e, DOMElement* parentTag);
    void CreateVariableElement(Variable& v, DOMElement* parentTag);

    static string GetVarType(VarType type);
    static XMLCh* TranscodeBinary(const string& str);
};
} // namespace RedatamLib

#endif // REDATAMLIB_XMLEXPORTER_HPP

