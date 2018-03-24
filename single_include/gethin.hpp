#ifndef GETHIN_HPP
#define GETHIN_HPP

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace gethin {

template <typename T>
class Optional {
 public:
  Optional() : m_present(false) {}
  Optional(const T &val) : m_present(true), m_val(val) {}
  bool isPresent() const { return m_present; }
  T get() const { return m_val; }

 private:
  bool m_present;
  T m_val;
};

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

class Flag : public Parameter_CRTP<Flag> {
 public:
  Flag() : m_supplied(false) {}
  bool value() const { return m_supplied; }
  Flag &value(bool supplied) {
    m_supplied = supplied;
    return *this;
  }

  void set(const std::string &arg) override {
    if (!arg.empty()) {
      throw std::invalid_argument("Boolean argument. Takes no value.");
    }
    m_isSet = true;
    value(true);
  }

  std::string usage() const override {
    std::ostringstream os;
    os << "-" << shortOpt().get() << " --" << longOpt().get();
    formattedHelp(os);
    return os.str();
  }

 private:
  bool m_supplied;
};

class Set : public Parameter_CRTP<Set> {
 public:
  Set() {}
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

    std::string delimiter = "";
    for (const std::string &item : m_alternatives) {
      os << delimiter << item;
      delimiter = ", ";
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
    m_isSet = true;
    value(arg);
  }

 private:
  std::string m_name;
  std::vector<std::string> m_alternatives;
  std::string m_value;
};

class String : public Parameter_CRTP<String> {
 public:
  String() {}
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
      throw std::invalid_argument("A value must be supplied to this option.");
    }
    m_isSet = true;
    value(arg);
  }

 private:
  std::string m_name;
  std::string m_value;
};

class OptionReader {
 public:
  OptionReader() : m_out(std::cout) {}
  OptionReader(std::initializer_list<Parameter *> opts,
               const std::string &appName = "<program>",
               std::ostream &out = std::cout)
      : m_appName(appName), m_out(out) {
    m_opts = std::vector<Parameter *>(opts);
  }
  OptionReader &with(Parameter *p) {
    m_opts.push_back(p);
    return *this;
  }
  OptionReader &with(std::vector<Parameter *> ps) {
    m_opts.insert(end(m_opts), begin(ps), end(ps));
    return *this;
  }

  void read(int argc, char *argv[]) {
    m_helpOpt.shortOpt('h').longOpt("help").help("Display usage information");
    m_opts.push_back(&m_helpOpt);
    m_usageOpt.shortOpt('u').longOpt("usage").help("Display usage information");
    m_opts.push_back(&m_usageOpt);
    std::vector<std::string> args(argv, argv + argc);
    preProcessInput(args);
    processInput(args);
  }

 private:
  std::string m_appName;
  std::ostream &m_out;
  std::vector<Parameter *> m_opts;
  Flag m_helpOpt;
  Flag m_usageOpt;

  static void preProcessInput(std::vector<std::string> &args) {
    for (size_t i = 0; i < args.size(); ++i) {
      std::string val = args[i];

      size_t p = val.find_first_of("=:");
      if (p != std::string::npos) {
        args[i] = val.substr(0, p);
        size_t secondLength = val.size() - p - 1;
        std::string second = val.substr(p + 1, secondLength);
        second.erase(remove(second.begin(), second.end(), '"'), second.end());
        args.insert(args.begin() + ++i, second);
      }

      else if (val[0] == '-' && val[1] != '-' && val.length() > 2) {
        args.erase(args.begin() + i--);
        for (size_t j = 1; j < val.size(); j++) {
          std::string extracted = "-";
          extracted += std::string(1, val[j]);
          args.insert(args.begin() + ++i, extracted);
        }
      }
    }
  }

  void processInput(std::vector<std::string> args) {
    for (size_t i = 0; i < args.size(); i++) {
      std::string str = args[i];
      if (isUsageOpt(str)) {
        printUsage();
        return;
      } else {
        parseInput(args, i);
      }
    }
    checkMandatoryOptions();
  }

  void checkMandatoryOptions() {
    for (Parameter *opt : m_opts) {
      if (opt->isMandatory() && !opt->isSet()) {
        throw std::invalid_argument(
            "The mandatory option '--" + opt->longOpt().get() + "' ('-" +
            opt->shortOpt().get() + "') was not supplied!");
      }
    }
  }

  void printUsage() {
    m_out << "Usage: " << m_appName << " [OPTION]" << std::endl;
    for (Parameter *p : m_opts) {
      m_out << p->usage() << std::endl;
    }
  }

  bool isUsageOpt(std::string str) {
    return isOption(str) && (m_helpOpt.matches(str) || m_usageOpt.matches(str));
  }

  void parseInput(std::vector<std::string> args, size_t i) {
    std::string val(args[i]);
    try {
      if (isOption(val)) {
        std::string data = "";
        size_t nextOptIdx = findNextOptIdx(args, i);
        if (nextOptIdx - i > 2) {
          throw std::invalid_argument("Two arguments given instead of one.");
        } else if (nextOptIdx - i == 2) {
          data = args[i + 1];
        }
        handleOpt(val, data);
      }
    } catch (const std::invalid_argument &e) {
      throw std::invalid_argument("Failed to parse option '" + val + "'. " +
                                  e.what());
    }
  }

  static size_t findNextOptIdx(std::vector<std::string> args,
                               size_t currentIdx) {
    for (size_t i = currentIdx + 1; i < args.size(); i++) {
      std::string candidate(args[i]);
      if (isOption(candidate)) {
        return i;
      }
    }
    return args.size();
  }

  static bool isOption(std::string val) {
    return val[0] == '/' || val[0] == '-';
  }

  void handleOpt(const std::string &enteredOpt, const std::string &arg) {
    for (Parameter *opt : m_opts) {
      if (opt->matches(enteredOpt)) {
        opt->set(arg);
        return;
      }
    }
    throw std::invalid_argument("Unknown option: '" + enteredOpt + "'");
  }
};
}  // namespace gethin
#endif  // GETHIN_HPP
