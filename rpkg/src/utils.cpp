#include <algorithm>    //  std::transform

#include "utils.hpp"

namespace RedatamLib
{
using std::invalid_argument;

string FindRootPath(const string& fileName)
{
    return fileName.substr(0, fileName.find_last_of('/') + 1);
}

string ReplaceRootPath(const string& rootPath, const string& fileName)
{
    string ret = rootPath;
    ret.append(fileName.substr(fileName.find_last_of('\\') + 1));

    return ret;
}

bool TryGetFileExtension(const string& fileName, string* output)
{
    try
    {
        string ext = GetFileExtension(fileName);
        *output = ext;
    }
    catch (const invalid_argument&)
    {
        return false;
    }
    
    return true;
}

string GetFileExtension(const string& fileName)
{
    auto extPos = fileName.find_last_of('.');
    ThrowIfBad<invalid_argument>(extPos != string::npos,
                                invalid_argument("Error: No file extension found."));

    string ext = fileName.substr(extPos);
    std::transform(ext.begin(), ext.end(), ext.begin(),
                                [](unsigned char c){ return std::tolower(c); });

    return ext;
}

} // namespace RedatamLib
