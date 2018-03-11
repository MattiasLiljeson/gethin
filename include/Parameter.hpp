#pragma once

#include <sstream>
#include <string>

using std::string;

namespace gethin {
class Parameter {
 public:
  virtual ~Parameter(){};
  virtual const string &longOpt() const = 0;
  virtual char shortOpt() const = 0;
  virtual void set(const string &arg) = 0;
  virtual string usage() const = 0;
};

template <typename T>
class Parameter_CRTP : public Parameter {
 public:
  Parameter_CRTP() : m_shortOpt(' '), m_longOpt(""), m_help("") {}
  virtual ~Parameter_CRTP(){};

  char shortOpt() const { return m_shortOpt; }
  T &shortOpt(char shortOpt) {
    m_shortOpt = shortOpt;
    return static_cast<T &>(*this);
  }

  const string &longOpt() const { return m_longOpt; }
  T &longOpt(const string &longOpt) {
    m_longOpt = longOpt;
    return static_cast<T &>(*this);
  }

  const string &help() const { return m_help; }
  void formattedHelp(std::ostringstream &os) const {
    for (size_t i = 0; i < m_help.size(); i += 72) {
      os << "\n\t"
         << m_help.substr(i, i + 72 > m_help.size() ? m_help.size() - i : 72);
    }
  }
  T &help(const string &help) {
    m_help = help;
    return static_cast<T &>(*this);
  }

  virtual void set(const string &arg) = 0;
  virtual string usage() const = 0;

 protected:
  char m_shortOpt;
  string m_longOpt;
  string m_help;
};
}  // namespace gethin