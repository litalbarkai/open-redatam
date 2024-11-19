#ifndef REDATAMLIB_UTILS_HPP
#define REDATAMLIB_UTILS_HPP

#include <cstring> // strerror
#include <string>
#include <system_error> // error_code, generic_category

namespace RedatamLib {
using std::error_code;
using std::generic_category;
using std::strerror;
using std::string;

string FindRootPath(const string &fileName);
string ReplaceRootPath(const string &rootPath, const string &fileName);

string GetFileExtension(const string &fileName);

bool TryGetFileExtension(const string &fileName, string *output);

template <typename E>
void ThrowIfBad(bool is_good_, error_code err_, const string &e_msg_) {
  if (!is_good_) {
    throw E(err_, e_msg_);
  }
}

template <typename E> void ThrowIfBad(bool is_good_, int errno_) {
  if (!is_good_) {
    throw E(error_code(errno_, generic_category()), strerror(errno_));
  }
}

template <typename E> void ThrowIfBad(bool is_good_, const E &e_) {
  if (!is_good_) {
    throw e_;
  }
}

template <typename E>
void ThrowIfBad(bool is_good_, const string &e_msg_) {
  if (!is_good_) {
    throw E(e_msg_);
  }
}
} // namespace RedatamLib

#endif // REDATAMLIB_UTILS_HPP
