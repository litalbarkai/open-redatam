#ifndef REDATAMLIB_XMLUTILS_HPP
#define REDATAMLIB_XMLUTILS_HPP

#include <string>

#include <xercesc/util/XMLString.hpp>

namespace RedatamLib
{
using std::string;
XERCES_CPP_NAMESPACE_USE

string TranscodeStr(const XMLCh* str);

} // namespace RedatamLib

#endif  //  REDATAMLIB_XMLUTILS_HPP