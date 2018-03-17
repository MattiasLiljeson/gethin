#pragma once

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>
#include "Parameter.hpp"

namespace gethin {

class Set : public Parameter_CRTP<Set> {
 public:
  Set() : m_mandatory(false), m_name(""), m_value("") {}
  bool mandatory() const { return m_mandatory; }
  Set &mandatory(bool mandatory) {
    m_mandatory = mandatory;
    return *this;
  }
  const std::string &name() const { return m_name; }
  Set &name(const std::string &name) {
    m_name = name;
    return *this;
  }
  const std::vector<std::string> &alternatives() const {
    return m_alternatives;
  }
  Set &alternatives(std::initializer_list<std::string> alternatives) {
    m_alternatives = std::vector<std::string>(alternatives);
    return *this;
  }
  const std::string &value() const { return m_value; }
  Set &value(const std::string &value) {
    m_value = value;
    return *this;
  }
  std::string usage() const override {
    std::ostringstream os;
    os << "-" << shortOpt().get() << " --" << longOpt().get() << " = {";
    for (std::string a : m_alternatives) {
      os << a << ", ";
    }
    os << "}";
    formattedHelp(os);
    return os.str();
  }

  void set(const std::string &arg) override {
    if (m_alternatives.empty()) {
      throw std::invalid_argument("Faulty option, no alternatives set!");
    }
    if (arg.empty() && m_mandatory) {
      throw std::invalid_argument(
          "Mandatory argument. Value must be supplied.");
    }
    if (find(m_alternatives.begin(), m_alternatives.end(), arg) ==
        m_alternatives.end()) {
      std::string message =
          "Argument must be one of: {'" + m_alternatives[0] + "'";
      for (size_t i = 1; i < m_alternatives.size(); i++) {
        message += ", '" + m_alternatives[i] + "'";
      }
      message += "}. The supplied argument was '" + arg + "'.";
      throw std::invalid_argument(message);
    }
    value(arg);
  }

 private:
  bool m_mandatory;
  std::string m_name;
  std::vector<std::string> m_alternatives;
  std::string m_value;
};
}  // namespace gethin