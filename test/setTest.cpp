#include <regex>
#include <string>
#include "ReHelper.hpp"
#include "catch.hpp"
#include "gethin.hpp"

TEST_CASE("Test Set option type") {
  gethin::Set set =
      gethin::Set()
          .shortOpt('a')
          .longOpt("palette-generation-algorithm")
          .mandatory(false)
          .alternatives({"kmeans", "iterative", "histogram", "mixed"})
          .help("Specify 'kmeans', 'iterative', 'histogram' or 'mixed' ");
  gethin::OptionReader optReader({&set});

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
  gethin::Set set = gethin::Set().shortOpt('c').longOpt("ccc");
  gethin::OptionReader optReader({&set});
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