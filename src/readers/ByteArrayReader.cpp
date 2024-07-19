#include <stdexcept>    //  std::out_of_range, std::length_error
#include <sstream>      //  std::ostringstream
#include <iterator>     //  std::ostream_iterator
#include <algorithm>    //  std::transform, std::all_of

#include <cctype>       //  std::isalnum

#include "ByteArrayReader.hpp"
#include "utils.hpp"

namespace RedatamLib
{
using std::out_of_range, std::length_error, std::ostringstream;

ByteArrayReader::ByteArrayReader(string filePath): m_currPos(0), m_endPos(0), m_data()
{
    m_data.push_back(byte(5));
    m_data.push_back(byte(0));
    m_data.push_back(byte(104));
    m_data.push_back(byte(101));
    m_data.push_back(byte(108));
    m_data.push_back(byte(108));
    m_data.push_back(byte(111));
    m_data.push_back(byte('\0'));
    m_data.push_back(byte(5));
    m_data.push_back(byte(0));
    m_data.push_back(byte('w'));
    m_data.push_back(byte('o'));
    m_data.push_back(byte('r'));
    m_data.push_back(byte('l'));
    m_data.push_back(byte('d'));
    m_data.push_back(byte('\0'));
    m_endPos = 16;
    //openfile
    //readfile
    //closefile
    //update endpos
}

ByteArrayReader::~ByteArrayReader()
{

}

void ByteArrayReader::SetPos(int newPos)
{
    ThrowIfBad<out_of_range>(0 <= newPos && newPos <= m_endPos,
                            out_of_range("Error: New position exceeds array bounds."));
    m_currPos = newPos;
}

void ByteArrayReader::MovePos(int bytes)
{
    SetPos(m_currPos + bytes);
}

bool ByteArrayReader::TryReadStr(string* output, bool filterByContent)
{
    return true;
}

bool ByteArrayReader::TryReadShortStr(string* output, bool filterByContent)
{
    size_t ogPos = m_currPos;

    try
    {
        int16_t len = ReadInt16LE();
        ThrowIfBad<length_error>(0 < len || 128 > len || m_currPos + len <= m_endPos,
                                length_error("Error: Invalid string length."));

        *output = ReadString(len + 1);
    }
    catch(const std::bad_alloc& e)
    {
        ThrowIfBad<std::bad_alloc>(false, e);
    }
    catch(const std::exception&)
    {
        SetPos(ogPos);
        return false;
    }
    
    return filterByContent ? IsValidStr(*output) : true;
}

// vector<size_t> ByteArrayReader::GetAllMatches(const vector<byte>& subArr)
// {
    
// }


bool ByteArrayReader::IsValidStr(string str)
{
    return ('\0' == str.back()) &&
        std::all_of(str.begin(), str.end() - 1, [](char c){
            return (std::isalnum(c) || ' ' == c || '-' == c || '_' == c);
        });
}

byte ByteArrayReader::ReadByte()
{
    byte ret = m_data[m_currPos];
    MovePos(1);

    return ret;
}

int16_t ByteArrayReader::ReadInt16LE()
{
    return static_cast<uint16_t>(ReadByte()) |
            static_cast<uint16_t>(ReadByte()) << 8;
}

int32_t ByteArrayReader::ReadInt32LE()
{
    return static_cast<uint32_t>(ReadInt16LE()) |
            static_cast<uint32_t>(ReadInt16LE()) << 16;
}

string ByteArrayReader::ReadString(size_t length)
{
    ostringstream oss;
    auto strStart = m_data.begin() + m_currPos;
    std::transform(strStart, strStart + length,
                    std::ostream_iterator<char>(oss),
                    [](byte b){return static_cast<char>(b);});
    MovePos(length);

    return oss.str();
}

} // namespace RedatamLib
