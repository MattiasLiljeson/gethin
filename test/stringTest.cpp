#include <regex>
#include <string>
#include "ReHelper.hpp"
#include "catch.hpp"
#include "gethin.hpp"

using gethin::OptionReader;
using gethin::String;

TEST_CASE("Test String option type") {
  String f = String()
                 .shortOpt('f')
                 .longOpt("foo")
                 .name("argument name, shown in help")
                 .help("some help text about foo");

  OptionReader optReader({&f});

  SECTION("String shortopt") {
    char* fake[2];
    fake[0] = (char*)"-f";
    fake[1] = (char*)"baz";
    optReader.read(2, fake);
    REQUIRE(f.value() == "baz");
  }

  SECTION("String longopt") {
    char* fake[1];
    fake[0] = (char*)"--foo=\"quux\"";
    optReader.read(1, fake);
    REQUIRE(f.value() == "quux");
  }

  SECTION("String without argument") {
    char* fake[1];
    fake[0] = (char*)"--foo";
    bool failed = false;
    try {
      optReader.read(1, fake);
    } catch (const std::invalid_argument& e) {
      failed = true;
    }
    REQUIRE(failed == true);
  }

  SECTION("String with two arguments") {
    char* fake[3];
    fake[0] = (char*)"--foo";
    fake[1] = (char*)"one";
    fake[2] = (char*)"two";
    bool failed = false;
    try {
      optReader.read(3, fake);
    } catch (const std::invalid_argument& e) {
      failed = true;
    }
    REQUIRE(failed == true);
  }

  SECTION("Test String usage, help text") {
    std::string usage = f.usage();
    std::smatch result;
    std::regex re(ReHelper::shortOptRe() + ReHelper::longOptRe() +
                  ReHelper::nameRe() + ReHelper::helpRe());
    std::regex_search(usage, result, re);
    REQUIRE(result.size() == 5);
    REQUIRE(result[1] == "f");
    REQUIRE(result[2] == "foo");
    REQUIRE(result[3] == "argument name, shown in help");
    REQUIRE(result[4] == "some help text about foo");
  }
}

// Mandatory option? yeah, we support that oxymoron!
TEST_CASE("Mandatory String") {
  String s = String().longOpt("bar").mandatory();
  OptionReader optReader({&s});
  char* fake[2];
  fake[0] = (char*)"--bar";
  fake[1] = (char*)"baz";
  optReader.read(2, fake);
  REQUIRE(s.value() == "baz");
}

// Mandatory option? yeah, we support that oxymoron!
TEST_CASE("Missing mandatory String") {
  String a = String().shortOpt('a');
  String s = String().longOpt("bar").mandatory();
  OptionReader optReader({&a, &s});
  char* fake[2];
  fake[0] = (char*)"-a";
  fake[1] = (char*)"baz";
  bool failed = false;
  try {
    optReader.read(2, fake);
  } catch (const std::invalid_argument& e) {
    failed = true;
  }
  REQUIRE(failed == true);
}