#include <ios>          //  std::ios_base::failure
#include <sstream>      //  std::ostringstream
#include <fstream>      //  std::ifstream, std::ios::binary
#include <iterator>     //  std::ostream_iterator, std::istreambuf_iterator
#include <algorithm>    //  std::transform, std::all_of, std::search

#include <cctype>       //  std::isalnum

#include "ByteArrayReader.hpp"
#include "utils.hpp"

namespace RedatamLib
{
ByteArrayReader::ByteArrayReader(string filePath) :
    m_currPos(0),
    m_endPos(0),
    m_data()
{
    std::ifstream fs(filePath, std::ios::binary);
    ThrowIfBad<std::ios_base::failure>(fs.is_open(),
        std::ios_base::failure("Error: Failed to open dictionary file."));

    m_data = vector<unsigned char>{std::istreambuf_iterator<char>(fs),
                                    std::istreambuf_iterator<char>()};
    m_endPos = fs.tellg();
}

ByteArrayReader::ByteArrayReader(const ByteArrayReader& other) : 
    m_currPos(0),
    m_endPos(other.m_endPos),
    m_data(other.m_data)
{}

size_t ByteArrayReader::GetPos() const
{
    return m_currPos;
}

size_t ByteArrayReader::GetEndPos() const
{
    return m_endPos;
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

void ByteArrayReader::MovePosTo(string subArr)
{
    const vector<unsigned char>& arr{subArr.begin(), subArr.end()};
    SetPos(FindNextMatch(arr, arr.size(), GetPos()));
}

// bool ByteArrayReader::TryReadStr(string* output, bool filterByContent)
// {
//     return true;
// }

bool ByteArrayReader::TryReadShortStr(string* output, bool filterByContent)
{
    size_t ogPos = m_currPos;

    try
    {
        int16_t len = ReadInt16LE();
        ThrowIfBad<length_error>(0 < len && 128 > len && m_currPos + len <= m_endPos,
            length_error("Error: Invalid string length."));

        *output = ReadString(len);
    }
    catch (const std::bad_alloc& e)
    {
        ThrowIfBad<std::bad_alloc>(false, e);
    }
    catch (const std::exception&)
    {
        SetPos(ogPos);
        return false;
    }
    
    return filterByContent ? IsValidStr(*output) : true;
}

string ByteArrayReader::ReadString(size_t length)
{
    std::ostringstream oss;
    auto strStart = m_data.begin() + m_currPos;
    std::copy(strStart, strStart + length, std::ostream_iterator<char>(oss));
    MovePos(length);

    return oss.str();
}

string ByteArrayReader::GetFormerString()
{
    size_t ogPos = GetPos();
    size_t offset = 2;  //  string length is indicated by 2 bytes
    MovePos(-offset);

    int16_t len = ReadInt16LE();
    while (len != offset - 2)
    {
        ++offset;
        MovePos(-3);    //  string length + 1 to move backwards
        len = ReadInt16LE();
    }

    return ReadString(len);
}

// vector<size_t> ByteArrayReader::GetAllMatches(const vector<unsigned char>& subArr)
// {
//     vector<size_t> ret;
//     size_t currPos = 0;
//     size_t len = subArr.size();

//     currPos = FindNextMatch(subArr, len, 0);
//     while (currPos <= m_endPos - len)
//     {
//         ret.push_back(currPos);
//         currPos = FindNextMatch(subArr, len, currPos + len);
//     }

//     return ret;
// }

size_t ByteArrayReader::FindNextMatch(const vector<unsigned char>& subArr, size_t len, size_t startPos)
{
    auto nextPosIt = std::search(m_data.begin() + startPos,
                                m_data.end() - len + 1,
                                subArr.begin(), subArr.end());
    
    return nextPosIt - m_data.begin();
}

bool ByteArrayReader::IsValidStr(string str)
{
    return std::all_of(str.begin(), str.end() - 1, [](char c){
            return (std::isalnum(c) || ' ' == c || '-' == c || '_' == c);
        });
}

byte ByteArrayReader::ReadByte()
{
    byte ret = static_cast<byte>(m_data[m_currPos]);
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

} // namespace RedatamLib
