//  replaces DataBlock.cs
#ifndef REDATAMLIB_FUZZYVARIABLEPARSER_HPP
#define REDATAMLIB_FUZZYVARIABLEPARSER_HPP

#include <vector>
#include <string>
#include <utility>          //  std::pair

#include "Variable.hpp"
#include "Entity.hpp"
#include "ByteArrayReader.hpp"

namespace RedatamLib {
using std::vector, std::string, std::pair;

class FuzzyVariableParser
{
public:
    FuzzyVariableParser(string filePath);
    FuzzyVariableParser(ByteArrayReader reader);
    ~FuzzyVariableParser() = default;

    FuzzyVariableParser(const FuzzyVariableParser&) = delete;
    FuzzyVariableParser& operator=(const FuzzyVariableParser&) = delete;

    void ParseAllVariables(vector<Entity>& entities);

private:
    ByteArrayReader m_reader;

    vector<pair<size_t, size_t>> GetSearchBounds(vector<Entity> entities);
    void ParseVariables(vector<Variable>* output,
                        pair<size_t, size_t> bounds);
    VarType ParseType();
    string ParseIdxFileName();
    size_t ParseDataSize(VarType type);
    vector<Tag> ParseTags();
    void ParseMissingAndNA(vector<Tag>* tags);
    size_t ParseDecimals();

    size_t GetSubstringLength(string delimiter);
};

} // namespace RedatamLib

#endif  //  REDATAMLIB_FUZZYVARIABLEPARSER_HPP