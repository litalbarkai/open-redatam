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

RedatamDatabase::RedatamDatabase(const string& fileName)
{
    OpenDictionary(fileName);
}

void RedatamDatabase::OpenDictionary(const string& fileName)
{
    string ext = GetFileExtension(fileName);

    if (".dic" == ext)
    {
        FuzzyEntityParser parser(fileName);
        m_entities = parser.ParseEntities();
    }
    else
    {
        ThrowIfBad<invalid_argument>(false,
            invalid_argument("Error: Dictionary file's extension must be .dic."));
    }
}
} // namespace RedatamLib
