#include <regex>
#include <string>
#include "ReHelper.hpp"
#include "catch.hpp"
#include "gethin.hpp"

TEST_CASE("Test String option type") {
  gethin::String f = gethin::String()
                         .shortOpt('f')
                         .longOpt("foo")
                         .mandatory(false)
                         .name("argument name, shown in help")
                         .help("some help text about foo");

  gethin::OptionReader optReader({&f});

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

  // The value will in this case be an empty string. An improvement could be
  // to throw instead as this is clearly erranous input.
  SECTION("String without argument") {
    char* fake[1];
    fake[0] = (char*)"--foo";
    optReader.read(1, fake);
    REQUIRE(f.value() == "");
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