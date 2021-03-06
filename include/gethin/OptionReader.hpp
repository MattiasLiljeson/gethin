#pragma once

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "Flag.hpp"
#include "Parameter.hpp"

namespace gethin {
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