#include "Variable.hpp"

#include "BitArrayReader.hpp"

namespace RedatamLib {
Variable::Variable()
    : m_name(""),
      m_type(NA),
      m_idxFileName(""),
      m_dataSize(0),
      m_filter(""),
      m_range(""),
      m_tags(),
      m_description(""),
      m_decimals(0) {}

Variable::Variable(const string& name, VarType type, const string& idxFileName,
                   size_t dataSize, const string& filter, const string& range,
                   vector<Tag> tags, const string& description, size_t decimals)
    : m_name(name),
      m_type(type),
      m_idxFileName(idxFileName),
      m_dataSize(dataSize),
      m_filter(filter),
      m_range(range),
      m_tags(tags),
      m_description(description),
      m_decimals(decimals) {
  ParseValues();
}

string Variable::GetName() const { return m_name; }

VarType Variable::GetType() const { return m_type; }

string Variable::GetFilePath() const { return m_idxFileName; }

size_t Variable::GetDataSize() const { return m_dataSize; }

string Variable::GetFilter() const { return m_filter; }

string Variable::GetRange() const { return m_range; }

vector<Tag> Variable::GetTags() const { return m_tags; }

string Variable::GetDescription() const { return m_description; }

size_t Variable::GetDecimals() const { return m_decimals; }

std::shared_ptr<void> Variable::GetValues() const { return m_values; }

void Variable::ParseValues() {
  ByteArrayReader reader(m_idxFileName);

  switch (m_type) {
    case BIN:
      ParseBIN(m_dataSize, reader);
      break;

    case CHR:
      ParseStrings(m_dataSize, reader);
      break;

    case DBL:
      ParseFloats(reader);
      break;

    case INT:
      ParseIntegers(2, reader);
      break;

    case LNG:
      ParseIntegers(4, reader);
      break;

    case PCK:
      ParsePCK(m_dataSize, reader);
      break;

    case NA:
    default:
      break;
  }
}

void Variable::ParseStrings(size_t length, ByteArrayReader reader) {
  vector<string>* vals = new vector<string>();
  struct Deleter {
    void operator()(void* ptr) const {
      delete static_cast<vector<string>*>(ptr);
    }
  };

  try {
    while (true) {
      vals->push_back(reader.ReadString(length));
    }
  } catch (const std::out_of_range&) {
  }

  m_values = shared_ptr<void>(static_cast<void*>(vals), Deleter());
}

void Variable::ParseIntegers(size_t size, ByteArrayReader reader) {
  vector<uint32_t>* vals = new vector<uint32_t>();
  struct Deleter {
    void operator()(void* ptr) const {
      delete static_cast<vector<uint32_t>*>(ptr);
    }
  };

  try {
    switch (size) {
      case 2:
        while (true) {
          vals->push_back(reader.ReadInt16LE());
        }
        break;

      case 4:
        while (true) {
          vals->push_back(reader.ReadInt32LE());
        }
        break;
    }
  } catch (const std::out_of_range&) {
  }

  m_values = shared_ptr<void>(static_cast<void*>(vals), Deleter());
}

void Variable::ParseFloats(ByteArrayReader reader) {
  vector<double>* vals = new vector<double>();
  struct Deleter {
    void operator()(void* ptr) const {
      delete static_cast<vector<double>*>(ptr);
    }
  };
  string temp;

  try {
    while (true) {
      temp = reader.ReadString(8);
      vals->push_back(
          *(reinterpret_cast<double*>(const_cast<char*>(temp.c_str()))));
    }
  } catch (const std::out_of_range&) {
  }

  m_values = shared_ptr<void>(static_cast<void*>(vals), Deleter());
}

void Variable::ParsePCK(size_t size, ByteArrayReader reader) {
  vector<uint32_t>* vals = new vector<uint32_t>();
  struct Deleter {
    void operator()(void* ptr) const {
      delete static_cast<vector<uint32_t>*>(ptr);
    }
  };
  uint32_t bits = 0;
  BitArrayReader bitReader(size);

  try {
    while (true) {
      bits = reader.ReadInt32LE();
      bitReader.ParseBits(vals, bits);
    }
  } catch (const std::out_of_range&) {
  }

  m_values = shared_ptr<void>(static_cast<void*>(vals), Deleter());
}

void Variable::ParseBIN(size_t size, ByteArrayReader reader) {
  vector<uint32_t>* vals = new vector<uint32_t>();
  struct Deleter {
    void operator()(void* ptr) const {
      delete static_cast<vector<uint32_t>*>(ptr);
    }
  };
  uint32_t bits = 0;
  BitArrayReader bitReader(size);

  try {
    while (true) {
      bits = reader.ReadInt32BE();
      bitReader.ParseBits(vals, bits);
    }
  } catch (const std::out_of_range&) {
  }

  m_values = shared_ptr<void>(static_cast<void*>(vals), Deleter());
}
}  // namespace RedatamLib
