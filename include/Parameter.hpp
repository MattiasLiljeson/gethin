#pragma once

#include <sstream>
#include <string>
#include "Optional.hpp"

namespace gethin {
class Parameter {
 public:
  virtual ~Parameter(){};
  virtual bool isMandatory() const = 0;
  virtual bool isSet() const = 0;
  virtual const Optional<char> shortOpt() const = 0;
  virtual const Optional<std::string> &longOpt() const = 0;
  virtual bool matches(const std::string &enteredOpt) const = 0;
  virtual void set(const std::string &arg) = 0;
  virtual std::string usage() const = 0;
};

template <typename T>
class Parameter_CRTP : public Parameter {
 public:
  Parameter_CRTP() : m_mandatory(false), m_isSet(false) {}
  virtual ~Parameter_CRTP(){};

  bool isMandatory() const { return m_mandatory; }
  T &mandatory() {
    m_mandatory = true;
    return static_cast<T &>(*this);
  }

  bool isSet() const { return m_isSet; }

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

  bool matches(const std::string &enteredOpt) const {
    bool longMatches = m_longOpt.isPresent() && longOptMatches(enteredOpt);
    bool shortMatches = m_shortOpt.isPresent() && shortOptMatches(enteredOpt);
    return longMatches || shortMatches;
  }

  bool longOptMatches(const std::string &enteredOpt) const {
    if (enteredOpt[0] == '/') {
      return enteredOpt.substr(1, std::string::npos) == m_longOpt.get();
    } else {
      return enteredOpt.substr(2, std::string::npos) == m_longOpt.get();
    }
  }

  bool shortOptMatches(const std::string &enteredOpt) const {
    return enteredOpt.length() == 2 && enteredOpt[1] == m_shortOpt.get();
  }

  virtual void set(const std::string &arg) = 0;
  virtual std::string usage() const = 0;

 protected:
  const int OUT_WIDTH = 72;
  bool m_mandatory;
  bool m_isSet;
  Optional<char> m_shortOpt;
  Optional<std::string> m_longOpt;
  Optional<std::string> m_help;
};
}  // namespace gethin