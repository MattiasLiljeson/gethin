#include "catch.hpp"
#include "gethin.hpp"

using gethin::OptionReader;
using gethin::Set;

TEST_CASE("Test Set option type") {
  Set set = Set()
                .shortOpt('a')
                .longOpt("palette-generation-algorithm")
                .mandatory(false)
                .alternatives({"kmeans", "iterative", "histogram", "mixed"})
                .help(
                    "Specify 'kmeans', 'iterative', 'histogram' or 'mixed' "
                    "where 'mixed' combines the results of 'histogram' and "
                    "'iterative'");
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