#include <algorithm>    //  std::transform

#include "utils.hpp"

namespace RedatamLib
{
using std::invalid_argument;

bool TryGetFileExtension(const string& filename, string* output)
{
    try
    {
        string ext = GetFileExtension(filename);
        *output = ext;
    }
    catch (const invalid_argument&)
    {
        return false;
    }
    
    return true;
}

string GetFileExtension(const string& filename)
{
    auto extPos = filename.find_last_of('.');
    ThrowIfBad<invalid_argument>(extPos != string::npos,
                                invalid_argument("Error: No file extension found."));

    string ext = filename.substr(extPos);
    std::transform(ext.begin(), ext.end(), ext.begin(),
                                [](unsigned char c){ return std::tolower(c); });

    return ext;
}

} // namespace RedatamLib
