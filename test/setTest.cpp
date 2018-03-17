#include <regex>
#include <string>
#include "ReHelper.hpp"
#include "catch.hpp"
#include "gethin.hpp"

using gethin::OptionReader;
using gethin::Set;

TEST_CASE("Test Set option type") {
  Set set = Set()
                .shortOpt('a')
                .longOpt("palette-generation-algorithm")
                .alternatives({"kmeans", "iterative", "histogram", "mixed"})
                .help("Specify 'kmeans', 'iterative', 'histogram' or 'mixed' ");
  OptionReader optReader({&set});

  SECTION("Set shortopt") {
    char* fake[2];
    fake[0] = (char*)"-a";
    fake[1] = (char*)"kmeans";
    optReader.read(2, fake);
    REQUIRE(set.value() == "kmeans");
  }

  SECTION("Set longopt") {
    char* fake[1];
    fake[0] = (char*)"--palette-generation-algorithm=\"mixed\"";
    optReader.read(1, fake);
    REQUIRE(set.value() == "mixed");
  }

  SECTION("Set without arguments") {
    char* fake[1];
    fake[0] = (char*)"-a";
    bool failed = false;
    try {
      optReader.read(1, fake);
    } catch (const std::invalid_argument& e) {
      failed = true;
    }
    REQUIRE(failed == true);
  }

  SECTION("Test Set usage, help text") {
    std::string usage = set.usage();
    std::smatch result;
    std::regex re(ReHelper::shortOptRe() + ReHelper::longOptRe() +
                  ReHelper::alternativesRe() + ReHelper::helpRe());
    std::regex_search(usage, result, re);
    REQUIRE(result.size() == 5);
    REQUIRE(result[1] == "a");
    REQUIRE(result[2] == "palette-generation-algorithm");
    REQUIRE(result[3] == "kmeans, iterative, histogram, mixed");
    REQUIRE(result[4] ==
            "Specify 'kmeans', 'iterative', 'histogram' or 'mixed' ");
  }
}

TEST_CASE("Skip specifying alternatives for a Set") {
  Set set = Set().shortOpt('c').longOpt("ccc");
  OptionReader optReader({&set});
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

// Mandatory option? yeah, we support that oxymoron!
TEST_CASE("Mandatory Set") {
  Set s = Set().longOpt("bar").mandatory().alternatives({"baz"});
  OptionReader optReader({&s});
  char* fake[2];
  fake[0] = (char*)"--bar";
  fake[1] = (char*)"baz";
  optReader.read(2, fake);
  REQUIRE(s.value() == "baz");
}

// Mandatory option? yeah, we support that oxymoron!
TEST_CASE("Missing mandatory Set") {
  Set a = Set().shortOpt('a').alternatives({"qux"});
  Set s = Set().longOpt("bar").mandatory().alternatives({"bar"});
  OptionReader optReader({&a, &s});
  char* fake[2];
  fake[0] = (char*)"-a";
  fake[1] = (char*)"qux";
  bool failed = false;
  try {
    optReader.read(2, fake);
  } catch (const std::invalid_argument& e) {
    failed = true;
  }
  REQUIRE(failed == true);
}