#include "XMLUtils.hpp"

namespace RedatamLib
{
     
string TranscodeStr(const XMLCh* str)
{
    char* cstr = XMLString::transcode(str);
    string ret = cstr;
    XMLString::release(&cstr);

    return ret;
}

} // namespace RedatamLib
