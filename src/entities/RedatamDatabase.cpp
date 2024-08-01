// #include <numeric>    //  std::accumulate
#include <string>       //  find_last_of, substr, npos
#include <stdexcept>    //  std::invalid_argument

#include <cctype>       //  std::tolower

#include "RedatamDatabase.hpp"
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
        // fuzzy parser
        // entity parser
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

// long RedatamDatabase::GetTotalDataItems()
// {
//     long ret = 0;

//     std::accumulate(m_entities.begin(), m_entities.end(), ret, [](auto entity) {
//     });
// }

// long RedatamDatabase::GetTotalRowsSize()
// {

// }

// vector<Entity> RedatamDatabase::GetEntitiesList()
// {
//     return vector<Entity>(m_entities);
// }


} // namespace RedatamLib
