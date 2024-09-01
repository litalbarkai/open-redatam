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
    //  throws std::ios_base::failure if fails to open file
    FuzzyVariableParser(const string& filePath);

    FuzzyVariableParser(ByteArrayReader reader, const string& rootPath);
    ~FuzzyVariableParser() = default;

    FuzzyVariableParser(const FuzzyVariableParser&) = delete;
    FuzzyVariableParser& operator=(const FuzzyVariableParser&) = delete;

    void ParseAllVariables(vector<Entity>& entities);

private:
    ByteArrayReader m_reader;
    string m_rootPath;

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