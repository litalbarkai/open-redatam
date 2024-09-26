#ifndef REDATAMLIB_CSVEXPORTER_HPP
#define REDATAMLIB_CSVEXPORTER_HPP

#include <string>
#include <mutex>

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

    void ExportAll(vector<Entity>& entities);

private:
    string m_path;
    std::mutex m_mtx;

    static void CreateVariablesLegend(std::mutex& mutex, Entity& e, const string& outputDirectory);
    static void CreateVariablesLabels(std::mutex& mutex, Entity& e, const string& outputDirectory);
    static void CreateVariablesData(std::mutex& mutex, Entity& e, const string& outputDirectory);
    static void ThreadExport(std::mutex& mutex,
                            size_t start, size_t end,
                            vector<Entity>& entities,
                            const string&  outputDirectory);
};
} // namespace RedatamLib

#endif // REDATAMLIB_CSVEXPORTER_HPP

