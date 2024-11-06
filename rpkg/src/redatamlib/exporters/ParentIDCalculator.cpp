#include "ParentIDCalculator.hpp"

namespace RedatamLib {
ParentIDCalculator::ParentIDCalculator(Entity *child)
    : m_child(child), m_currID(0), m_currLimit(0) {}

size_t ParentIDCalculator::GetParentID(size_t currRow) {
  while (currRow > m_currLimit) {
    m_currLimit = m_child->GetPTRData();
    ++m_currID;
  }

  return m_currID;
}

}  // namespace RedatamLib
