// #include <numeric>    //  std::accumulate
#include <string>       //  find_last_of, substr, npos
#include <stdexcept>    //  std::invalid_argument
#include <algorithm>    //  std::transform

#include <cctype>       //  std::tolower

#include "RedatamDatabase.hpp"
#include "utils.hpp"

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

// static
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

// static
string RedatamDatabase::GetFileExtension(const string& filename)
{
    auto extPos = filename.find_last_of('.');
    ThrowIfBad<invalid_argument>(extPos != string::npos,
                                invalid_argument("Error: No file extension found."));

    string ext = filename.substr(extPos);
    std::transform(ext.begin(), ext.end(), ext.begin(),
                                [](unsigned char c){ return std::tolower(c); });

    return ext;
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
