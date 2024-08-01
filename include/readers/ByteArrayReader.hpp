//  replaces DataBlock.cs
#ifndef REDATAMLIB_BYTEARRREADER_HPP
#define REDATAMLIB_BYTEARRREADER_HPP

#include <vector>
#include <string>
#include <cstddef>  //  std::byte, std::size_t
#include <cstdint>  //  int16_t, int32_t

namespace RedatamLib {
using std::vector, std::string, std::byte, std::size_t;

class ByteArrayReader
{
public:
    ByteArrayReader(string filePath);  //  throws???
    //ctor for partial blocks???
    ~ByteArrayReader() = default;

    ByteArrayReader(const ByteArrayReader&) = delete;
    ByteArrayReader& operator=(const ByteArrayReader&) = delete;

    size_t GetPos() const;
    
    //  throws std::out_of_range
    void SetPos(int newPos);
    void MovePos(int bytes);

    bool TryReadStr(string* output, bool filterByContent = true);
    //  short str's length is between 0 and 128 bytes
    bool TryReadShortStr(string* output, bool filterByContent = true);

    //  returns a list of positions of matches to "subArr" inside the array
    vector<size_t> GetAllMatches(const vector<unsigned char>& subArr);

private:
    vector<unsigned char> m_data;
    size_t m_currPos;
    size_t m_endPos;

    size_t FindNextMatch(const vector<unsigned char>& subArr, size_t len, size_t startPos);
    bool IsValidStr(string str);

    //  throws std::out_of_range; LE = little-endian, BE = big-endian
    byte ReadByte();
    int16_t ReadInt16LE();
    int32_t ReadInt32LE();
    string ReadString(size_t length);
};

} // namespace RedatamLib

#endif  //  REDATAMLIB_BYTEARRREADER_HPP