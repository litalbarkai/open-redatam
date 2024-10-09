#include <algorithm>  //  std::transform

// avoid using boost
// we only need to create directories and write simple files
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>  // mkdir
#include <unistd.h>    // access
#endif

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

#ifdef _WIN32
bool exists(const std::string& path) 
{
    DWORD attr = GetFileAttributes(path.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES);
}

bool create_directories(const std::string& path)
{
    return CreateDirectory(path.c_str(), NULL) ||
        GetLastError() == ERROR_ALREADY_EXISTS;
}
#else
bool exists(const std::string& path) {
    return access(path.c_str(), F_OK) != -1;
}

bool create_directories(const std::string& path)
{
    mode_t mode = 0755;
    return mkdir(path.c_str(), mode) == 0 || errno == EEXIST;
}
#endif

} // namespace RedatamLib
