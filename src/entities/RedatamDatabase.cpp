// #include <numeric>    //  std::accumulate
#include <string>       //  find_last_of, substr, npos
#include <stdexcept>    //  std::invalid_argument

#include <cctype>       //  std::tolower

#include "RedatamDatabase.hpp"
#include "FuzzyEntityParser.hpp"
#include "utils.hpp"    //  ThrowIfBad, GetFileExtension

namespace RedatamLib
{
using std::string, std::vector, std::invalid_argument;

RedatamDatabase::RedatamDatabase(const string& filename)
{
    OpenDictionary(filename);

    // validate files (frmMain)
}

RedatamDatabase::~RedatamDatabase()
{

}

void RedatamDatabase::OpenDictionary(const string& filename)
{
    string ext = GetFileExtension(filename);

    if (".dic" == ext)
    {
        FuzzyEntityParser parser(filename);
        m_entities = parser.ParseEntities();
    }
    else if (".dicx" == ext)
    {
        //  xml parser
    }
    else
    {
        ThrowIfBad<invalid_argument>(false,
            invalid_argument("Error: Dictionary file's extension must be .dic or .dicx ."));
    }
}
} // namespace RedatamLib
