#include <iostream> // cerr, cout, endl
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h> // mkdir
#include <unistd.h>   // access
#endif

#include "RedatamDatabase.hpp"
#include "utils.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::string;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cerr << "Usage: redatam input.dic(x) output-csv/" << endl;
    return 1;
  }

  string dicFilePath = argv[1];
  string outputDirPath = argv[2];

  if (!RedatamLib::Exists(dicFilePath)) {
    cerr << "Error: Input dictionary file does not exist." << endl;
    return 1;
  }

  if (!RedatamLib::CreateDirectories(outputDirPath)) {
    cerr << "Error: Failed to create output directory." << endl;
    return 1;
  }

  try {
    RedatamLib::RedatamDatabase db(dicFilePath);
    db.ExportCSVFiles(outputDirPath);
    db.ExportSummary(outputDirPath);
    cout << "Conversion successful!" << endl;
  } catch (const exception &e) {
    cout << "\033[1;31mConversion failed.\033[0m" << endl;
    return 1;
  }

  return 0;
}