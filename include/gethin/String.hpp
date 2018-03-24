#pragma once

#include <stdexcept>
#include <string>
#include "Parameter.hpp"

namespace gethin {

class String : public Parameter_CRTP<String> {
 public:
  String(){}
  const std::string &name() const { return m_name; }
  String &name(const std::string &name) {
    m_name = name;
    return *this;
  }
  const std::string &value() const { return m_value; }
  String &value(const std::string &value) {
    m_value = value;
    return *this;
  }
  std::string usage() const override {
    std::ostringstream os;
    os << "-" << shortOpt().get() << " --" << longOpt().get() << " = ";
    os << "<" << name() << ">";
    formattedHelp(os);
    return os.str();
  }

  void set(const std::string &arg) override {
    if (arg.empty()) {
      throw std::invalid_argument(
          "A value must be supplied to this option.");
    }
    m_isSet = true;
    value(arg);
  }

 private:
  std::string m_name;
  std::string m_value;
};
}  // namespace gethin