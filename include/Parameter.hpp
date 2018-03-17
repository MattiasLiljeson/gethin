#pragma once

#include <sstream>
#include <string>

#include "Optional.hpp"

namespace gethin {
class Parameter {
 public:
  virtual ~Parameter(){};
  virtual const Optional<std::string> &longOpt() const = 0;
  virtual const Optional<char> shortOpt() const = 0;
  virtual bool matches(const std::string& enteredOpt) = 0;
  virtual void set(const std::string &arg) = 0;
  virtual std::string usage() const = 0;
};

template <typename T>
class Parameter_CRTP : public Parameter {
 public:
  Parameter_CRTP(){}
  virtual ~Parameter_CRTP(){};

  const Optional<char> shortOpt() const { return m_shortOpt; }
  T &shortOpt(char shortOpt) {
    m_shortOpt = Optional<char>(shortOpt);
    return static_cast<T &>(*this);
  }

  const Optional<std::string> &longOpt() const { return m_longOpt; }
  T &longOpt(const std::string &longOpt) {
    m_longOpt = Optional<std::string>(longOpt);
    return static_cast<T &>(*this);
  }

  const Optional<std::string> &help() const { return m_help; }
  void formattedHelp(std::ostringstream &os) const {
    size_t length = m_help.get().size();
    for (size_t fromIdx = 0; fromIdx < length; fromIdx += OUT_WIDTH) {
      int toIdx = fromIdx + OUT_WIDTH > length ? length - fromIdx : OUT_WIDTH;
      os << "\n\t" << m_help.get().substr(fromIdx, toIdx);
    }
  }
  T &help(const std::string &help) {
    m_help = Optional<std::string>(help);
    return static_cast<T &>(*this);
  }

  bool matches(const std::string& enteredOpt){
      bool longMatches = m_longOpt.isPresent() &&
       enteredOpt.substr(2, std::string::npos) == m_longOpt.get();
      bool shortMatches = m_shortOpt.isPresent() &&
       enteredOpt.length() == 2 && enteredOpt[1] == m_shortOpt.get();

       return longMatches || shortMatches;
  }

  virtual void set(const std::string &arg) = 0;
  virtual std::string usage() const = 0;

 protected:
  const int OUT_WIDTH = 72;
  Optional<char> m_shortOpt;
  Optional<std::string> m_longOpt;
  Optional<std::string> m_help;
};
}  // namespace gethin