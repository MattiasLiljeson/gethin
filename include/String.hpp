#pragma once

#include <string>

#include "Parameter.hpp"

using std::string;

namespace gethin {

class String : public Parameter {
 public:
  bool mandatory() const { return m_mandatory; }
  String &mandatory(bool mandatory) {
    m_mandatory = mandatory;
    return *this;
  }
  const string &name() const { return m_name; }
  String &name(const string &name) {
    m_name = name;
    return *this;
  }
  const string &value() const { return m_value; }
  String &value(const string &value) {
    m_value = value;
    return *this;
  }
  string usage() const override {
    std::ostringstream os;
    os << "-" << shortOpt() << " --" << longOpt() << " = ";
    os << (mandatory() ? string("[") + name() + "]" : name());
    // os<<"}\n"<<"\t";
    formattedHelp(os);
    return os.str();
  }

  void set(const string &arg) override {
    if (arg.empty() && m_mandatory) {
      throw std::invalid_argument(
          "Mandatory argument. Value must be supplied.");
    }
    value(arg);
  }

 private:
  bool m_mandatory;
  string m_name;
  string m_value;
};
}  // namespace gethin