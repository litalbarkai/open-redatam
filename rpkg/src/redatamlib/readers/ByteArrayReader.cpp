#include "ByteArrayReader.hpp"
#include "utils.hpp"

#include <algorithm> // search
#include <fstream>   // ifstream
#include <iterator>  // ostream_iterator, istreambuf_iterator
#include <sstream>   // ostringstream

namespace RedatamLib {

using std::all_of;
using std::bad_alloc;
using std::copy;
using std::exception;
using std::ifstream;
using std::isalnum;
using std::istreambuf_iterator;
using std::ostream_iterator;
using std::ostringstream;
using std::out_of_range;
using std::search;
using std::string;
using std::transform;

ByteArrayReader::ByteArrayReader() : m_currPos(0), m_endPos(0) {}

ByteArrayReader::ByteArrayReader(const string &filePath)
    : m_data(), m_currPos(0), m_endPos(0) {
  string eMsg = "Error: Failed to open file: ";
  ifstream fs(filePath, std::ios::binary);
  if (!fs.is_open()) {
    // Extract the filename part
#ifdef _WIN32
    string filename = filePath.substr(filePath.find_last_of("\\") + 1);
    string directory = filePath.substr(0, filePath.find_last_of("\\") + 1);
#else
    string filename = filePath.substr(filePath.find_last_of("/") + 1);
    string directory = filePath.substr(0, filePath.find_last_of("/") + 1);
#endif

    // Try with lowercase extension, keeping directory intact
    string lowerFilename = filename;
    auto pos = lowerFilename.find_last_of('.');
    if (pos != string::npos) {
      transform(lowerFilename.begin() + pos, lowerFilename.end(),
                lowerFilename.begin() + pos, ::tolower);
    }
    fs.open(directory + lowerFilename, std::ios::binary);
    if (!fs.is_open()) {
      // Try with uppercase extension
      string upperFilename = filename;
      if (pos != string::npos) {
        transform(upperFilename.begin() + pos, upperFilename.end(),
                  upperFilename.begin() + pos, ::toupper);
      }
      fs.open(directory + upperFilename, std::ios::binary);
      if (!fs.is_open()) {
        // Try with uppercase filename and lowercase extension
        string upperFileNameLowerExt = filename;
        if (pos != string::npos) {
          transform(upperFileNameLowerExt.begin(),
                    upperFileNameLowerExt.begin() + pos,
                    upperFileNameLowerExt.begin(), ::toupper);
          transform(upperFileNameLowerExt.begin() + pos,
                    upperFileNameLowerExt.end(),
                    upperFileNameLowerExt.begin() + pos, ::tolower);
        }
        fs.open(directory + upperFileNameLowerExt, std::ios::binary);
        if (!fs.is_open()) {
          // Try with uppercase filename and extension
          string upperFileNameUpperExt = filename;
          transform(upperFileNameUpperExt.begin(), upperFileNameUpperExt.end(),
                    upperFileNameUpperExt.begin(), ::toupper);
          fs.open(directory + upperFileNameUpperExt, std::ios::binary);
          if (!fs.is_open()) {
            // Try with lowercase filename and uppercase extension
            string lowerFileNameUpperExt = filename;
            if (pos != string::npos) {
              transform(lowerFileNameUpperExt.begin(),
                        lowerFileNameUpperExt.begin() + pos,
                        lowerFileNameUpperExt.begin(), ::tolower);
              transform(lowerFileNameUpperExt.begin() + pos,
                        lowerFileNameUpperExt.end(),
                        lowerFileNameUpperExt.begin() + pos, ::toupper);
            }
            fs.open(directory + lowerFileNameUpperExt, std::ios::binary);
            if (!fs.is_open()) {
              throw std::ios_base::failure(eMsg + filePath);
            }
          }
        }
      }
    }
  }

  m_data = vector<unsigned char>((istreambuf_iterator<char>(fs)),
                                 istreambuf_iterator<char>());
  m_endPos = fs.tellg();
}

size_t ByteArrayReader::GetPos() const { return m_currPos; }

size_t ByteArrayReader::GetEndPos() const { return m_endPos; }

void ByteArrayReader::SetPos(int newPos) {
  ThrowIfBad<out_of_range>(
      0 <= newPos && size_t(newPos) <= m_endPos,
      out_of_range("Error: New position exceeds array bounds."));
  m_currPos = newPos;
}

void ByteArrayReader::MovePos(int bytes) { SetPos(m_currPos + bytes); }

void ByteArrayReader::MovePosTo(const string &subArr) {
  const vector<unsigned char> arr{subArr.begin(), subArr.end()};
  SetPos(FindNextMatch(arr, arr.size(), GetPos()));
}

bool ByteArrayReader::TryReadStr(string *output, bool filterByContent) {
  size_t ogPos = m_currPos;

  try {
    uint16_t len = ReadInt16LE();
    ThrowIfBad<length_error>(0 < len && 128 > len &&
                                 m_currPos + len <= m_endPos,
                             length_error("Error: Invalid string length."));

    *output = ReadString(len);
  } catch (const bad_alloc &e) {
    ThrowIfBad<bad_alloc>(false, e);
  } catch (const exception &) {
    SetPos(ogPos);
    return false;
  }

  return filterByContent ? IsValidStr(*output) : true;
}

string ByteArrayReader::ReadString(size_t length) {
  if (m_currPos + length > m_data.size()) {
    throw out_of_range("Attempt to read past the end of the buffer");
  }
  ostringstream oss;
  auto strStart = m_data.begin() + m_currPos;
  copy(strStart, strStart + length, ostream_iterator<char>(oss));
  MovePos(length);

  return oss.str();
}

string ByteArrayReader::GetFormerString() {
  size_t offset = 2; // string length is indicated by 2 bytes
  MovePos(-offset);

  uint16_t len = ReadInt16LE();
  while (len != offset - 2) {
    ++offset;
    MovePos(-3); // string length + 1 to move backwards
    len = ReadInt16LE();
  }

  return ReadString(len);
}

size_t ByteArrayReader::FindNextMatch(const vector<unsigned char> &subArr,
                                      size_t len, size_t startPos) {
  auto nextPosIt = search(m_data.begin() + startPos, m_data.end() - len + 1,
                          subArr.begin(), subArr.end());
  return nextPosIt - m_data.begin();
}

bool ByteArrayReader::IsValidStr(const string &str) {
  return all_of(str.begin(), str.end() - 1, [](char c) {
    return (isalnum(c) || ' ' == c || '-' == c || '_' == c);
  });
}

unsigned char ByteArrayReader::ReadByte() {
  if (m_currPos >= m_data.size()) {
    throw out_of_range("Attempt to read beyond buffer size");
  }

  unsigned char ret = m_data[m_currPos];
  MovePos(1);

  return ret;
}

uint16_t ByteArrayReader::ReadInt16LE() {
  uint16_t a = static_cast<uint16_t>(ReadByte());
  uint16_t b = static_cast<uint16_t>(ReadByte()) << 8;
  return a | b;
}

uint32_t ByteArrayReader::ReadInt32LE() {
  uint32_t a = static_cast<uint32_t>(ReadInt16LE());
  uint32_t b = static_cast<uint32_t>(ReadInt16LE()) << 16;
  return a | b;
}

uint16_t ByteArrayReader::ReadInt16BE() {
  uint16_t a = static_cast<uint16_t>(ReadByte()) << 8;
  uint16_t b = static_cast<uint16_t>(ReadByte());
  return a | b;
}

uint32_t ByteArrayReader::ReadInt32BE() {
  uint32_t a = static_cast<uint32_t>(ReadInt16BE()) << 16;
  uint32_t b = static_cast<uint32_t>(ReadInt16BE());
  return a | b;
}

} // namespace RedatamLib
