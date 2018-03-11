#pragma once

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::vector;

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

class Flag : public Parameter {
 public:
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

class Set : public Parameter {
 public:
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
    if (arg.empty() && m_mandatory) {
      throw std::invalid_argument(
          "Mandatory argument. Value must be supplied.");
    }
    if (find(m_alternatives.begin(), m_alternatives.end(), arg) ==
        m_alternatives.end()) {
      string message = "Argument must be one of: {'" + m_alternatives[0] + "'";
      for (int i = 1; i < m_alternatives.size(); i++) {
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

class OptionReader {
 public:
  OptionReader() {}
  OptionReader(vector<Parameter *> opts) : m_opts(opts) {}
  OptionReader &with(Parameter *p) {
    m_opts.push_back(p);
    return *this;
  }
  OptionReader &with(vector<Parameter *> ps) {
    m_opts.insert(end(m_opts), begin(ps), end(ps));
    return *this;
  }

  void read(int argc, char *argv[]) {
    vector<string> args(argv, argv + argc);
    std::cout << "Before: " << args.size() << std::endl;
    preProcessInput(args);

    std::cout << "After: " << args.size() << std::endl;
    processInput(args);
  }

 private:
  vector<Parameter *> m_opts;

  static void preProcessInput(vector<string> &args) {
    for (int i = 0; i < args.size(); ++i) {
      string val = args[i];

      size_t p = val.find('=');
      if (p != string::npos) {
        args[i] = val.substr(0, p);
        size_t secondLength = val.size() - p - 1;
        string second = val.substr(p + 1, secondLength);
        second.erase(remove(second.begin(), second.end(), '"'), second.end());
        args.insert(args.begin() + ++i, second);
      }

      else if (val[0] == '-' && val[1] != '-' && val.length() > 2) {
        args.erase(args.begin() + i--);
        for (size_t j = 1; j < val.size(); j++) {
          string extracted = "-";
          extracted += string(1, val[j]);
          args.insert(args.begin() + ++i, extracted);
        }
      }
    }
  }

  void processInput(vector<string> args) {
    for (size_t i = 0; i < args.size(); i++) {
      string str = args[i];
      if (isUsageOpt(str)) {
        printUsage();
        return;
      } else {
        parseInput(args, i);
      }
    }
  }

  void printUsage() {
    std::cout << "Usage: <program> [OPTION]" << std::endl;
    for (Parameter *p : m_opts) {
      std::cout << p->usage() << std::endl;
    }
  }

  static bool isUsageOpt(string str) {
    if (str == "-h" || str == "--help" || str == "-u" || str == "--usage") {
      return true;
    }
    return false;
  }

  void parseInput(vector<string> args, size_t i) {
    string val(args[i]);
    std::cout << "Val: " << val << std::endl;
    try {
      if (isOption(val)) {
        string data = "";
        size_t nextOptIdx = findNextOptIdx(args, i);
        std::cout << "\t" << i << " : " << nextOptIdx << " : " << nextOptIdx - i
                  << std::endl;
        if (nextOptIdx - i > 2) {
          throw std::invalid_argument("Two arguments given instead of one.");
        } else if (nextOptIdx - i == 2) {
          data = args[i + 1];
        }
        handleOpt(val, data);
      }
    } catch (const std::invalid_argument &e) {
      throw std::invalid_argument("Failed to parse option: '" + val + "': " +
                                  e.what());
    }
  }
  static bool isOption(string val) { return val[0] == '/' || val[0] == '-'; }

  static size_t findNextOptIdx(vector<string> args, size_t currentIdx) {
    for (size_t i = currentIdx + 1; i < args.size(); i++) {
      string candidate(args[i]);
      if (isOption(candidate)) {
        std::cout << "\t" << candidate << "@" << i << std::endl;
        return i;
      }
    }
    return args.size();
  }

  void handleOpt(const string &enteredOpt, const string &arg) {
    bool found = false;
    for (Parameter *opt : m_opts) {
      if (enteredOpt.substr(2, string::npos) == opt->longOpt() ||
          (enteredOpt.length() == 2 && enteredOpt[1] == opt->shortOpt())) {
        std::cout << "\tentered:\t'" << enteredOpt << "'" << std::endl;
        std::cout << "\tmatched:\t'" << opt->longOpt() << "' or '"
                  << opt->shortOpt() << "'" << std::endl;
        std::cout << "\tdata:\t'" << arg << "'" << std::endl;
        opt->set(arg);
        found = true;
      }
    }
    if (!found) {
      throw std::invalid_argument("Unknown option: '" + enteredOpt + "'");
    }
  }
};
}  // namespace gethin