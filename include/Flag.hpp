#pragma once

#include <string>
#include "Parameter.hpp"

using std::string;

namespace gethin {

class Flag : public Parameter_CRTP<Flag> {
 public:
 Flag() : m_supplied(false){}
  bool value() const { return m_supplied; }
  Flag &value(bool supplied) {
    m_supplied = supplied;
    return *this;
  }

  void set(const string &arg) override {
    if (!arg.empty()) {
      throw std::invalid_argument("Boolean argument. Takes no value.");
    }
    value(true);
  }

  string usage() const override {
    std::ostringstream os;
    os << "-" << shortOpt() << " --" << longOpt();
    // os<<"}\n"<<"\t"<<formattedHelp(os);
    formattedHelp(os);
    return os.str();
  }

 private:
  bool m_supplied;
};
}  // namespace gethin