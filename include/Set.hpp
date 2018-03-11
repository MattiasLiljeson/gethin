#pragma once

#include "Parameter.hpp"

#include <algorithm>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace gethin {

class Set : public Parameter_CRTP<Set> {
 public:
  Set() : m_mandatory(false), m_name(""), m_value("") {}
  bool mandatory() const { return m_mandatory; }
  Set &mandatory(bool mandatory) {
    m_mandatory = mandatory;
    return *this;
  }
  const string &name() const { return m_name; }
  Set &name(const string &name) {
    m_name = name;
    return *this;
  }
  const vector<string> &alternatives() const { return m_alternatives; }
  Set &alternatives(const vector<string> &alternatives) {
    m_alternatives = alternatives;
    return *this;
  }
  const string &value() const { return m_value; }
  Set &value(const string &value) {
    m_value = value;
    return *this;
  }
  string usage() const override {
    std::ostringstream os;
    os << "-" << shortOpt() << " --" << longOpt() << " = {";
    for (string a : m_alternatives) {
      os << a << ", ";
    }
    os << "}";
    formattedHelp(os);
    return os.str();
  }

  void set(const string &arg) override {
    if (m_alternatives.empty()) {
      throw std::invalid_argument("Faulty option, no alternatives set!");
    }
    if (arg.empty() && m_mandatory) {
      throw std::invalid_argument(
          "Mandatory argument. Value must be supplied.");
    }
    if (find(m_alternatives.begin(), m_alternatives.end(), arg) ==
        m_alternatives.end()) {
      string message = "Argument must be one of: {'" + m_alternatives[0] + "'";
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
  string m_name;
  vector<string> m_alternatives;
  string m_value;
};
}  // namespace gethin