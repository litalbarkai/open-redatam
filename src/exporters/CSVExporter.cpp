#include <filesystem>
#include <fstream>          //  std::ofstream
#include <iostream>         //  std::endl

#include "CSVExporter.hpp"
#include "ParentIDCalculator.hpp"

namespace RedatamLib
{
using std::ofstream, std::endl;
namespace fs = std::filesystem;

CSVExporter::CSVExporter(const string& outputDirectory) : m_path(outputDirectory)
{
    if ('/' != m_path.back())
    {
        m_path.append("/");
    }

    fs::path p = m_path;
    if (!fs::exists(p.append("Labels/")))
    {
        create_directories(p);
    }
}

void CSVExporter::CreateVariablesLegend(Entity& e)
{
    string entityName = e.GetName();
    ofstream fs(m_path + "Labels/" + entityName + "-VARIABLES.csv");

    fs << "variable;label" << endl
        << entityName << "_REF_ID;Identifier for " << entityName << endl;
    
    if (!e.GetParentName().empty())
    {
        fs << e.GetParentName() << "_REF_ID;Parent entity Id" << endl;
    }
    
    for (Variable& v : e.GetVariables())
    {
        fs << v.GetName() << ";" << v.GetDescription() << endl;
    }
}

void CSVExporter::CreateVariablesLabels(Entity& e)
{
    string path = m_path + "Labels/" + e.GetName() + "-";

    for (Variable& v : e.GetVariables())
    {
        if (!v.GetTags().empty())
        {
            ofstream fs(path + v.GetName() + "-LABELS.csv");

            fs << v.GetName() << ";label" << endl;
            for (Tag t : v.GetTags())
            {
                fs << t.first << ";" << t.second << endl;
            }
        }
    }
}

void CSVExporter::CreateVariablesData(Entity& e)
{
    ofstream fs(m_path + e.GetName() + ".csv");
    bool hasParent = !e.GetParentName().empty();
    ParentIDCalculator pIDCalc(&e);

    fs << e.GetName() << "_REF_ID";
    if (hasParent)
    {
        fs << ";" << e.GetParentName() << "_REF_ID";
    }

    for (Variable& v : e.GetVariables())
    {
        fs << ";" << v.GetName();
    }

    for (size_t row = 1; row < 1 + e.GetRowsCount(); ++row)
    {
        fs << endl << row;
        if (hasParent)
        {
            fs << ";" << pIDCalc.GetParentID(row);
        }

        for (Variable& v : e.GetVariables())
        {
            switch (v.GetType())
            {
            case BIN:
            case PCK:
            case INT:
            case LNG:
                fs << ";" << (*(static_cast<vector<uint32_t>*>(v.GetValues().get())))[row - 1];
                break;

            case CHR:
                fs << ";" << (*(static_cast<vector<string>*>(v.GetValues().get())))[row - 1];
                break;

            case DBL:
                fs << ";" << (*(static_cast<vector<double>*>(v.GetValues().get())))[row - 1];
                break;
            }
        }
    }
}

} // namespace RedatamLib