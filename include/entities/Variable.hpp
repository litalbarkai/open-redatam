#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include <string>
#include <vector>
#include <utility>          //  std::pair

#include "ByteArrayReader.hpp"

namespace RedatamLib {
using std::vector, std::string, std::pair;

enum VarType {BIN, CHR, DBL, INT, LNG, PCK, NA};
using Tag = pair<string, string>;   //  Tag = <Key, Value>

class Variable {
public:
    explicit Variable();
    explicit Variable(string name,
                        VarType type,
                        string idxFileName,
                        size_t dataSize,
                        string filter,
                        string range,
                        vector<Tag> tags,
                        string description,
                        size_t decimals = 0);
    ~Variable() = default;

private:
    string m_name;
    VarType m_type;
    string m_idxFileName;
    size_t m_dataSize;
    string m_filter;
    string m_range;
    vector<Tag> m_tags;
    string m_description;
    size_t m_decimals;
    void* m_values;

    void ParseValues();
    void ParseStrings(size_t length, ByteArrayReader reader);
};

} // namespace RedatamLib

#endif // VARIABLE_HPP
