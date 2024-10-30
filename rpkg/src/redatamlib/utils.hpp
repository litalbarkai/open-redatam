#ifndef REDATAMLIB_UTILS_HPP
#define REDATAMLIB_UTILS_HPP

#include <cstring>    //  std::strerror
#include <stdexcept>  //  std::invalid_argument
#include <string>
#include <system_error>  //  std::error_code, std::generic_category

namespace RedatamLib {
using std::string;

string FindRootPath(const string& fileName);
string ReplaceRootPath(const string& rootPath, const string& fileName);

//  Throws std::invalid_argument
string GetFileExtension(const string& fileName);

bool TryGetFileExtension(const string& fileName, string* output);

}  // namespace RedatamLib

#endif  //  REDATAMLIB_UTILS_HPP
