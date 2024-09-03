#include <string>
#include <iostream>     //  std::cerr, std::cout, std::endl
#include <filesystem>   //  exists, create_directories

#include "RedatamDatabase.hpp"




#include <ctime>




using std::string, std::cerr, std::cout, std::endl;
namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: redatam input.dic(x) output-csv/" << endl;
        return 1;
    }

    string dicFilePath = argv[1];
    string outputDirPath = argv[2];

    if (!fs::exists(dicFilePath))
    {
        cerr << "Error: Input dicionary file does not exist." << endl;
        return 1;
    }

    try
    {
        auto ts = time(NULL);
        RedatamLib::RedatamDatabase db(dicFilePath);
        auto te = time(NULL);
        cout << te - ts << endl;
        // db.ExportCSVFiles(outputDirPath);
        // db.ExportSummary(outputDirPath);
        cout << "Conversion successful!" << endl;
    }
    catch (const std::exception& e)
    {
        cout << "\033[1;31mConversion failed.\033[0m" << endl;
        return 1;
    }

    return 0;
}