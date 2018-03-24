#pragma once

#include <iostream>

namespace gethin {
template <typename T>
class Optional {
 public:
  Optional() : m_present(false) {}
  Optional(const T& val) : m_present(true), m_val(val) {}
  bool isPresent() const { return m_present; }
  T get() const { return m_val; }

 private:
  bool m_present;
  T m_val;
};
}  // namespace gethin