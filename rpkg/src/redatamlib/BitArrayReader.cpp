#include "BitArrayReader.hpp"

#include <bitset>
#include <vector>

namespace RedatamLib {
BitArrayReader::BitArrayReader(size_t dataSize)
    : m_varSize(dataSize),
      m_remainderSize(0),
      m_mask(CreateMask(dataSize)),
      m_data(0),
      m_remainder(0) {}

std::bitset<32> BitArrayReader::CreateMask(size_t size) {
  std::bitset<32> mask = 0;

  for (size_t i = 1; i < size + 1; ++i) {
    mask[32 - i] = 1;
  }

  return mask;
}

void BitArrayReader::ParseBits(std::vector<uint32_t>* results, uint32_t data) {
  std::bitset<32> remainderMask = CreateMask(m_remainderSize);
  std::bitset<32> bits = data;
  m_data = m_remainder |
           (((~remainderMask << m_remainderSize) & bits) >> m_remainderSize);

  std::bitset<32> val;
  size_t bitIdx = 0;

  for (bitIdx = 0; bitIdx < 1 + 32 - m_varSize; bitIdx = bitIdx + m_varSize) {
    val = ((m_mask >> bitIdx) & m_data) >> (32 - bitIdx - m_varSize);
    results->push_back(val.to_ulong());
  }

  m_remainder = ((m_mask >> bitIdx) & m_data) << bitIdx |
                ((remainderMask >> (32 - m_remainderSize)) & bits)
                    << (bitIdx - m_remainderSize);
  m_remainderSize = m_remainderSize + (32 % m_varSize);
  while (m_remainderSize >= m_varSize) {
    val = (m_mask & m_remainder) >> (32 - m_varSize);
    results->push_back(val.to_ulong());

    m_remainder <<= m_varSize;
    m_remainderSize -= m_varSize;
  }
}
}  // namespace RedatamLib
