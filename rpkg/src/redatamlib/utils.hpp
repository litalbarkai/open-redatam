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

template <typename E>
void ThrowIfBad(bool is_good_, std::error_code err_, const string& e_msg_) {
  if (!is_good_) {
    throw E(err_, e_msg_);
  }
}

template <typename E>
void ThrowIfBad(bool is_good_, int errno_) {
  if (!is_good_) {
    throw E(std::error_code(errno_, std::generic_category()),
            string(std::strerror(errno_)));
  }
}

template <typename E>
void ThrowIfBad(bool is_good_, const E& e_) {
  if (!is_good_) {
    throw e_;
  }
}

}  // namespace RedatamLib

#endif  //  REDATAMLIB_UTILS_HPP
