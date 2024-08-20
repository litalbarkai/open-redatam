//  replaces DataBlock.cs
#ifndef REDATAMLIB_BYTEARRREADER_HPP
#define REDATAMLIB_BYTEARRREADER_HPP

#include <stdexcept>    //  std::out_of_range, std::length_error
#include <vector>
#include <string>
#include <cstddef>      //  std::byte, std::size_t
#include <cstdint>      //  uint16_t, uint32_t

namespace RedatamLib {
using std::vector, std::string, std::byte, std::size_t;
using std::out_of_range, std::length_error;

class ByteArrayReader
{
public:
    ByteArrayReader(string filePath);  //  throws???
    ByteArrayReader(const ByteArrayReader& other);
    //ctor for partial blocks???
    ~ByteArrayReader() = default;

    ByteArrayReader& operator=(const ByteArrayReader&) = delete;

    size_t GetPos() const;
    size_t GetEndPos() const;
    
    //  throws std::out_of_range
    void SetPos(int newPos);
    void MovePos(int bytes);
    void MovePosTo(string subArr);

    bool TryReadStr(string* output, bool filterByContent = true);
    //  short str's length is between 0 and 128 bytes
    bool TryReadShortStr(string* output, bool filterByContent = true);
    
    //  throws std::out_of_range
    string ReadString(size_t length);

    //  throws std::out_of_range if there isn't such string
    //  used to find variable names
    string GetFormerString();

    //  returns a list of positions of matches to "subArr" inside the array
    // vector<size_t> GetAllMatches(const vector<unsigned char>& subArr);

    //  throws std::out_of_range; LE = little-endian, BE = big-endian
    byte ReadByte();
    uint16_t ReadInt16LE();
    uint32_t ReadInt32LE();
    uint16_t ReadInt16BE();
    uint32_t ReadInt32BE();

private:
    vector<unsigned char> m_data;
    size_t m_currPos;
    size_t m_endPos;

    size_t FindNextMatch(const vector<unsigned char>& subArr, size_t len, size_t startPos);
    bool IsValidStr(string str);
};

} // namespace RedatamLib

#endif  //  REDATAMLIB_BYTEARRREADER_HPP