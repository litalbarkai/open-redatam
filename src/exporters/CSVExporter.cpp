#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

#include "CSVExporter.hpp"
#include "ParentIDCalculator.hpp"
#include "utils.hpp"

namespace RedatamLib {
using std::cout;
using std::endl;
using std::ofstream;
using std::ostringstream;
using std::vector;
using std::thread;
using std::ref;
using std::lock_guard;
using std::mutex;
using std::ios_base;

CSVExporter::CSVExporter(const string &outputDirectory)
    : m_path(outputDirectory) {
  if (m_path.back() != '/') {
    m_path.append("/");
  }

  string labels_path = m_path + "Labels/";
  if (!Exists(labels_path)) {
    CreateDirectories(labels_path);
  }
}

void CSVExporter::ExportAll(vector<Entity> &entities) {
  size_t numThreads = thread::hardware_concurrency();
  numThreads = std::min(entities.size(), numThreads);

  size_t chunkSize = entities.size() / numThreads;
  vector<thread> threads;

  for (size_t i = 0; i < numThreads; ++i) {
    size_t start = i * chunkSize;
    size_t end = (i == numThreads - 1) ? entities.size() : start + chunkSize;

    threads.emplace_back(&CSVExporter::ThreadExport, this, start, end, ref(entities));
  }

  for (auto &t : threads) {
    t.join();
  }
}

void CSVExporter::CreateVariablesLegend(Entity &e) const {
  string entityName = e.GetName();
  ostringstream os;

  os << "variable;label" << endl
     << entityName << "_REF_ID;Identifier for " << entityName << endl;

  if (!e.GetParentName().empty()) {
    os << e.GetParentName() << "_REF_ID;Parent entity Id" << endl;
  }

  for (Variable &v : *(e.GetVariables().get())) {
    os << v.GetName() << ";" << v.GetDescription() << endl;
  }

  lock_guard<mutex> lock(m_mtx);
  ofstream fs(m_path + "Labels/" + entityName + "-VARIABLES.csv");
  ThrowIfBad<ios_base::failure>(fs.is_open(), ios_base::failure("Error: Failed to create file."));
  fs << os.str();
}

void CSVExporter::CreateVariablesLabels(Entity &e) const {
  string path = m_path + "Labels/" + e.GetName() + "-";

  for (Variable &v : *(e.GetVariables().get())) {
    if (!v.GetTags().empty()) {
      ostringstream os;

      os << v.GetName() << ";label" << endl;
      for (Tag t : v.GetTags()) {
        os << t.first << ";" << t.second << endl;
      }

      lock_guard<mutex> lock(m_mtx);
      ofstream fs(path + v.GetName() + "-LABELS.csv");
      ThrowIfBad<ios_base::failure>(fs.is_open(), ios_base::failure("Error: Failed to create file."));
      fs << os.str();
    }
  }
}

void CSVExporter::CreateVariablesData(Entity &e) const {
  ostringstream os;
  bool hasParent = !e.GetParentName().empty();
  ParentIDCalculator pIDCalc(&e);

  os << e.GetName() << "_REF_ID";
  if (hasParent) {
    os << ";" << e.GetParentName() << "_REF_ID";
  }

  for (Variable &v : *(e.GetVariables().get())) {
    os << ";" << v.GetName();
  }

  size_t rows = 1 + e.GetRowsCount();
  for (size_t row = 1; row < rows; ++row) {
    os << endl << row;
    if (hasParent) {
      os << ";" << pIDCalc.GetParentID(row);
    }

    for (Variable &v : *(e.GetVariables().get())) {
      switch (v.GetType()) {
        case BIN:
        case PCK:
        case INT:
        case LNG:
          os << ";"
             << (*(static_cast<vector<uint32_t> *>(
                    v.GetValues().get())))[row - 1];
          break;

        case CHR:
          os << ";"
             << (*(static_cast<vector<string> *>(
                    v.GetValues().get())))[row - 1];
          break;

        case DBL:
          os << ";"
             << (*(static_cast<vector<double> *>(
                    v.GetValues().get())))[row - 1];
          break;

        default:
          break;
      }
    }
  }

  lock_guard<mutex> lock(m_mtx);
  ofstream fs(m_path + e.GetName() + ".csv");
  ThrowIfBad<ios_base::failure>(fs.is_open(), ios_base::failure("Error: Failed to create file."));
  fs << os.str();
}

void CSVExporter::ThreadExport(size_t start, size_t end,
                               vector<Entity> &entities) const {
  for (size_t i = start; i < end; ++i) {
    cout << "Exporting " << entities[i].GetName() << "..." << endl;
    CreateVariablesLegend(entities[i]);
    CreateVariablesLabels(entities[i]);
    CreateVariablesData(entities[i]);
  }
}

}  // namespace RedatamLib
