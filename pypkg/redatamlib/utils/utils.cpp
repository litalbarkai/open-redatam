#include "utils.hpp"

#include <algorithm> // min
#include <iostream>  // cerr

namespace RedatamLib {

using std::invalid_argument;

string FindRootPath(const string &fileName) {
#ifdef _WIN32
  size_t pos = fileName.find_last_of('\\');
#else
  size_t pos = fileName.find_last_of('/');
#endif
  return (pos == string::npos) ? "" : fileName.substr(0, pos + 1);
}

string GetFileName(const string &fullPath) {
  size_t posSlash = fullPath.find_last_of('/');
  size_t posBackslash = fullPath.find_last_of('\\');

  // Use parentheses around std::min to disambiguate min on Windows
  size_t pos = (posSlash == string::npos) ? posBackslash
               : (posBackslash == string::npos)
                   ? posSlash
                   : (std::min)(posSlash, posBackslash);
  return (pos == string::npos) ? fullPath : fullPath.substr(pos + 1);
}

string ReplaceRootPath(const string &rootPath, const string &fileName) {
  string normalizedRootPath = rootPath;
  if (normalizedRootPath.back() != '/' && normalizedRootPath.back() != '\\') {
    normalizedRootPath.append("/");
  }

  // Remove everything before the last '/' or '\'
  string normalizedFileName = fileName;
  size_t pos = normalizedFileName.find_last_of("/\\");
  if (pos != string::npos) {
    normalizedFileName = normalizedFileName.substr(pos + 1);
  }

  string fullPath = normalizedRootPath + normalizedFileName;
  return fullPath;
}

string GetFileExtension(const string &fileName) {
  size_t pos = fileName.find_last_of('.');
  if (pos == string::npos) {
    ThrowIfBad<invalid_argument>(false, "Error: File has no extension.");
  }
  return fileName.substr(pos);
}

bool TryGetFileExtension(const string &fileName, string *output) {
  size_t pos = fileName.find_last_of('.');
  if (pos == string::npos) {
    return false;
  }
  *output = fileName.substr(pos);
  return true;
}

} // namespace RedatamLib
