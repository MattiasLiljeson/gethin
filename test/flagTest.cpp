#include <regex>
#include <string>
#include "ReHelper.hpp"
#include "catch.hpp"
#include "gethin.hpp"

TEST_CASE("Test Flag option type") {
  gethin::Flag b = gethin::Flag().shortOpt('b').longOpt("bar").help(
      "some help text about bar");
  gethin::OptionReader optReader({&b});

  SECTION("Flag shortopt") {
    char* fake[1];
    fake[0] = (char*)"-b";
    optReader.read(1, fake);
    REQUIRE(b.value() == true);
  }

  SECTION("Flag longopt") {
    char* fake[1];
    fake[0] = (char*)"--bar";
    optReader.read(1, fake);
    REQUIRE(b.value() == true);
  }

  SECTION("Test Flag usage, help text") {
    std::string usage = b.usage();
    std::smatch result;
    std::regex re(ReHelper::shortOptRe() + ReHelper::longOptRe() +
                  ReHelper::helpRe());
    std::regex_search(usage, result, re);
    REQUIRE(result.size() == 4);
    REQUIRE(result[1] == "b");
    REQUIRE(result[2] == "bar");
    REQUIRE(result[3] == "some help text about bar");
  }
}