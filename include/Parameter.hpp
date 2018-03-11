#pragma once

#include <sstream>
#include <string>

using std::string;

namespace gethin {
class Parameter {
 public:
  Parameter() : m_shortOpt(' '), m_longOpt(""), m_help("") {}
  char shortOpt() const { return m_shortOpt; }
  Parameter &shortOpt(char shortOpt) {
    m_shortOpt = shortOpt;
    return *this;
  }
  const string &longOpt() const { return m_longOpt; }
  Parameter &longOpt(const string &longOpt) {
    m_longOpt = longOpt;
    return *this;
  }
  const string &help() const { return m_help; }
  void formattedHelp(std::ostringstream &os) const {
    for (size_t i = 0; i < m_help.size(); i += 72) {
      os << "\n\t"
         << m_help.substr(i, i + 72 > m_help.size() ? m_help.size() - i : 72);
    }
  }
  Parameter &help(const string &help) {
    m_help = help;
    return *this;
  }
  virtual void set(const string &arg) = 0;
  virtual string usage() const = 0;

 private:
  char m_shortOpt;
  string m_longOpt;
  string m_help;
};
}  // namespace gethin