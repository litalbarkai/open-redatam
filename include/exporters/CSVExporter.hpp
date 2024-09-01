#ifndef REDATAMLIB_CSVEXPORTER_HPP
#define REDATAMLIB_CSVEXPORTER_HPP

#include <string>

#include "Entity.hpp"
#include "Variable.hpp"

namespace RedatamLib
{
using std::string;

class CSVExporter {
public:
    explicit CSVExporter(const string& outputDirectory);
    ~CSVExporter() = default;

    CSVExporter(const CSVExporter&) = delete;
    CSVExporter& operator=(const CSVExporter&) = delete;

    void CreateVariablesLegend(Entity& e);
    void CreateVariablesLabels(Entity& e);
    void CreateVariablesData(Entity& e);

private:
    string m_path;
};
} // namespace RedatamLib

#endif // REDATAMLIB_CSVEXPORTER_HPP

