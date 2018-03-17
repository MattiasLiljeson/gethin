#pragma once

#include <sstream>
#include <string>

#include "Optional.hpp"

namespace gethin {
class Parameter {
 public:
  virtual ~Parameter(){};
  virtual const std::string &longOpt() const = 0;
  virtual char shortOpt() const = 0;
  virtual void set(const std::string &arg) = 0;
  virtual std::string usage() const = 0;
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

  const std::string &longOpt() const { return m_longOpt; }
  T &longOpt(const std::string &longOpt) {
    m_longOpt = longOpt;
    return static_cast<T &>(*this);
  }

  const std::string &help() const { return m_help; }
  void formattedHelp(std::ostringstream &os) const {
    for (size_t i = 0; i < m_help.size(); i += 72) {
      os << "\n\t"
         << m_help.substr(i, i + 72 > m_help.size() ? m_help.size() - i : 72);
    }
  }
  T &help(const std::string &help) {
    m_help = help;
    return static_cast<T &>(*this);
  }

  virtual void set(const std::string &arg) = 0;
  virtual std::string usage() const = 0;

 protected:
  char m_shortOpt;
  std::string m_longOpt;
  std::string m_help;
};
}  // namespace gethin