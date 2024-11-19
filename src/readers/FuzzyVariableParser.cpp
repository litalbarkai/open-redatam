#include "FuzzyVariableParser.hpp"
#include "utils.hpp" //  GetFileExtension, ThrowIfBad

#include <regex> // regex, regex_search, smatch
#include <thread>

namespace RedatamLib {
using std::exception;
using std::invalid_argument;
using std::lock_guard;
using std::mutex;
using std::out_of_range;
using std::ref;
using std::regex;
using std::regex_search;
using std::smatch;
using std::stoi;
using std::string;
using std::thread;
using std::vector;

FuzzyVariableParser::FuzzyVariableParser(const string &filePath)
    : m_reader(filePath), m_rootPath(FindRootPath(filePath)) {}

FuzzyVariableParser::FuzzyVariableParser(ByteArrayReader reader,
                                         const string &rootPath)
    : m_reader(reader), m_rootPath(rootPath) {}

void FuzzyVariableParser::ParseAllVariables(vector<Entity> &entities) {
  if (entities.empty()) {
    throw invalid_argument("Error: The entities vector is empty.");
  }

  vector<pair<size_t, size_t>> searchBounds = GetSearchBounds(entities);

  // R-devel suggestion: Default to using all available hardware concurrency
  size_t maxThreads = thread::hardware_concurrency();
  size_t numThreads = std::min(entities.size(), maxThreads);

  if (numThreads == 0) {
    numThreads = 1;
  }

  size_t chunkSize = entities.size() / numThreads;

  vector<thread> threads;
  for (size_t i = 0; i < numThreads; ++i) {
    size_t start = i * chunkSize;
    size_t end = (i == numThreads - 1) ? entities.size() : start + chunkSize;

    threads.push_back(thread(ThreadParseVars, ref(m_mtx), start, end,
                             ref(entities), searchBounds, m_rootPath,
                             m_reader));
  }

  for (auto &t : threads) {
    t.join();
  }
}

vector<pair<size_t, size_t>>
FuzzyVariableParser::GetSearchBounds(vector<Entity> &entities) {
  vector<pair<size_t, size_t>> ret;

  if (entities.empty()) {
    throw invalid_argument("Error: The entities vector is empty.");
  }

  for (size_t i = 0; i < entities.size() - 1; ++i) {
    ret.push_back(
        {entities[i].GetBounds().second, entities[i + 1].GetBounds().first});
  }
  ret.push_back({entities.back().GetBounds().second, m_reader.GetEndPos()});

  return ret;
}

//  static
VarType FuzzyVariableParser::ParseType(ByteArrayReader *reader) {
  string type = reader->ReadString(3);
  reader->MovePos(1);

  if ("BIN" == type) {
    return BIN;
  } else if ("CHR" == type) {
    return CHR;
  } else if ("DBL" == type) {
    return DBL;
  } else if ("INT" == type) {
    return INT;
  } else if ("LNG" == type) {
    return LNG;
  } else if ("PCK" == type) {
    return PCK;
  }

  return NA;
}

//  static
string FuzzyVariableParser::ParseIdxFileName(const string &rootPath,
                                             ByteArrayReader *reader) {
  size_t ogPos = reader->GetPos();

  reader->MovePos(1);
  reader->MovePosTo("'");
  size_t len = reader->GetPos() - ogPos - 1;

  reader->SetPos(ogPos + 1);
  string ret = reader->ReadString(len);
  reader->MovePos(1); //  "'"

  ret = ReplaceRootPath(rootPath, ret);

  return ret;
}

//  static
size_t FuzzyVariableParser::ParseDataSize(VarType type,
                                          ByteArrayReader *reader) {
  size_t len = 0;
  string str;
  regex re("\\d+");
  smatch match;

  switch (type) {
  case DBL:
    return 8;
    break;

  case LNG:
    return 4;
    break;

  case INT:
    return 2;
    break;

  case BIN:
  case CHR:
  case PCK:
    reader->MovePos(6); //  " SIZE "
    len = GetSubstringLength("", reader);
    str = reader->ReadString(len);

    // Extract the numeric part from the string
    if (regex_search(str, match, re)) {
      try {
        return stoi(match.str());
      } catch (const invalid_argument &e) {
        throw invalid_argument("Invalid argument: " + string(e.what()) +
                               " for string: '" + str + "'");
      } catch (const out_of_range &e) {
        throw out_of_range("Out of range: " + string(e.what()) +
                           " for string: '" + str + "'");
      }
    } else {
      throw invalid_argument("No numeric part found in string: '" + str + "'");
    }
    break;

  case NA:
  default:
    break;
  }

  return 0; // Default return value if no case matches
}

//  static
vector<Tag> FuzzyVariableParser::ParseTags(ByteArrayReader *reader) {
  vector<Tag> ret;

  string discard("");
  reader->TryReadStr(
      &discard); //  data type identifier ("INTEGER", "STRING", "REAL")

  size_t ogPos = reader->GetPos();
  size_t len = reader->ReadInt16LE();
  while (reader->GetPos() - ogPos < len + 2) {
    size_t keyLen = GetSubstringLength(" ", reader);
    string key = reader->ReadString(keyLen);

    reader->MovePos(1); //  " "

    size_t valLen = GetSubstringLength("\t", reader);
    string value = reader->ReadString(valLen);

    ret.push_back(Tag(key, value));

    reader->MovePos(1); //  "\t"
  }

  return ret;
}

//  static
void FuzzyVariableParser::ParseMissingAndNA(vector<Tag> *tags,
                                            ByteArrayReader *reader) {
  string missing = "MISSING";
  string na = "NOTAPPLICABLE";
  size_t len = reader->ReadInt16LE();

  if (0 != len) {
    size_t ogPos = reader->GetPos();
    size_t maxPos = reader->GetEndPos();

    //  find labels' searching limit by next "DATASET"
    try {
      reader->MovePosTo("DATASET");
      maxPos = reader->GetPos();
      reader->SetPos(ogPos);
    } catch (const exception &) {
      reader->SetPos(ogPos);
    }

    try {
      reader->MovePosTo(missing);
      ThrowIfBad(maxPos > reader->GetPos(),
                 out_of_range("Label doesn't belong to current variable."));

      reader->MovePos(missing.size() + 1); //  missing + " "
      size_t keyLen1 = GetSubstringLength(" ", reader);
      string key1 = reader->ReadString(keyLen1);
      tags->push_back(Tag(key1, missing));
    } catch (const exception &) {
      reader->SetPos(ogPos);
    }

    try {
      reader->MovePosTo(na);
      ThrowIfBad(maxPos > reader->GetPos(),
                 out_of_range("Label doesn't belong to current variable."));

      reader->MovePos(na.size() + 1); //  na + " "
      size_t keyLen2 = std::min(GetSubstringLength("", reader),
                                GetSubstringLength(" ", reader));
      string key2 = reader->ReadString(keyLen2);
      tags->push_back(Tag(key2, na));
    } catch (const exception &) {
      reader->SetPos(ogPos);
    }
  }
}

//  static
size_t FuzzyVariableParser::ParseDecimals(ByteArrayReader *reader) {
  reader->MovePos(10); //  " DECIMALS "
  size_t len =
      std::min(GetSubstringLength("", reader), GetSubstringLength(" ", reader));
  return stoi(reader->ReadString(len));
}

//  static
size_t FuzzyVariableParser::GetSubstringLength(string delimiter,
                                               ByteArrayReader *reader) {
  if (delimiter.empty()) {
    delimiter = '\0';
  }

  size_t ogPos = reader->GetPos();
  reader->MovePosTo(delimiter);
  size_t ret = reader->GetPos() - ogPos;
  reader->SetPos(ogPos);

  return ret;
}

//  static
void FuzzyVariableParser::ParseVariables(shared_ptr<vector<Variable>> output,
                                         pair<size_t, size_t> bounds,
                                         const string &rootPath,
                                         ByteArrayReader reader) {
  try {
    reader.SetPos(bounds.first);

    while (true) {
      reader.MovePosTo("DATASET");
      ThrowIfBad(
          reader.GetPos() < bounds.second,
          out_of_range("Error: DATASET doesn't belong to current entity."));

      reader.MovePos(-2); //  "DATASET" length indicator
      string varName = reader.GetFormerString();

      reader.MovePos(10); //  "DATASET" + length indicator + " "
      VarType type = ParseType(&reader);
      string idxFileName = ParseIdxFileName(rootPath, &reader);
      size_t dataSize = ParseDataSize(type, &reader);
      string filter;
      if (!reader.TryReadStr(&filter)) {
        reader.MovePos(2);
      }
      string range;
      if (!reader.TryReadStr(&range)) {
        reader.MovePos(2);
      }
      vector<Tag> tags = ParseTags(&reader);
      string description;
      if (!reader.TryReadStr(&description, false)) {
        reader.MovePos(2);
      }
      ParseMissingAndNA(&tags, &reader);
      size_t decimals = 0;
      if (DBL == type) {
        decimals = ParseDecimals(&reader);
      }

      output->push_back(Variable(varName, type, idxFileName, dataSize, filter,
                                 range, tags, description, decimals));
    }
  } catch (const out_of_range &) {
  }
}

//  static
void FuzzyVariableParser::ThreadParseVars(
    mutex &mtx, size_t start, size_t end, vector<Entity> &entities,
    vector<pair<size_t, size_t>> searchBounds, const string &rootPath,
    ByteArrayReader reader) {
  for (size_t i = start; i < end; ++i) {
    shared_ptr<vector<Variable>> vars(new vector<Variable>);
    ParseVariables(vars, searchBounds[i], rootPath, reader);

    lock_guard<mutex> lock(mtx);
    entities[i].AttachVariables(vars);
  }
}
} // namespace RedatamLib
