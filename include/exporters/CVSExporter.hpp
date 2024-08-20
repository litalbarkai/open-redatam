#ifndef REDATAMLIB_CSVEXPORTER_HPP
#define REDATAMLIB_CSVEXPORTER

namespace RedatamLib
{
class CSVExporter {
public:
    explicit CSVExporter();
    ~CSVExporter() = default;

    CSVExporter(const CSVExporter&) = delete;
    CSVExporter& operator=(const CSVExporter&) = delete;
private:
};
} // namespace RedatamLib

#endif // REDATAMLIB_CSVEXPORTER_HPP

