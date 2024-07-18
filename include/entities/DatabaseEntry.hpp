#ifndef REDATAMLIB_DATABASEENTRY_HPP
#define REDATAMLIB_DATABASEENTRY_HPP

#include <string>
#include <vector>
#include <utility>  //  std::pair
#include <memory>   //  std::shared_pointer

#include "RedatamDatabase.hpp"

namespace RedatamLib
{

class DatabaseEntry {
public:
  DatabaseEntry() = default;
  ~DatabaseEntry() = default;

private:
  std::string m_inl;
  std::string m_caption;
  std::string m_dictFolder;
  std::shared_ptr<RedatamDatabase> m_database;
  std::vector<std::pair<std::string, std::string>> m_filesQueue;
};

} // namespace RedatamLib

#endif // REDATAMLIB_DATABASEENTRY_HPP
