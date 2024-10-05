#ifndef REDATAMLIB_PARENTIDCALCULATOR_HPP
#define REDATAMLIB_PARENTIDCALCULATOR_HPP

#include "Entity.hpp"

namespace RedatamLib {
class ParentIDCalculator {
 public:
  explicit ParentIDCalculator(Entity* child);
  ~ParentIDCalculator() = default;

  ParentIDCalculator(const ParentIDCalculator&) = delete;
  ParentIDCalculator& operator=(const ParentIDCalculator&) = delete;

  size_t GetParentID(size_t currRow);

 private:
  Entity* m_child;
  size_t m_currID;
  size_t m_currLimit;
};
}  // namespace RedatamLib

#endif  // REDATAMLIB_PARENTIDCALCULATOR_HPP
