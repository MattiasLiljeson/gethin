#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "gethin.hpp"

using gethin::Flag;
using gethin::OptionReader;
using gethin::Parameter;
using gethin::Set;
using gethin::String;

TEST_CASE("Test gethin getopt lib") {
  String f = String()
                 .shortOpt('f')
                 .longOpt("foo")
                 .mandatory(false)
                 .name("argument name, shown in help")
                 .help("some help text about foo");

  Flag b = Flag().shortOpt('b').longOpt("bar").help("some help text about bar");

  Set pga = Set()
                .shortOpt('a')
                .longOpt("palette-generation-algorithm")
                .mandatory(false)
                .alternatives(
                    vector<string>{"kmeans", "iterative", "histogram", "mixed"})
                .help(
                    "Specify 'kmeans', 'iterative', 'histogram' or 'mixed' "
                    "where 'mixed' combines the results of 'histogram' and "
                    "'iterative'");

  OptionReader optReader(vector<Parameter*>{&f, &b, &pga});

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

  SECTION("Set shortopt") {
    char* fake[2];
    fake[0] = (char*)"-a";
    fake[1] = (char*)"kmeans";
    optReader.read(2, fake);
    REQUIRE(pga.value() == "kmeans");
  }

  SECTION("Set longopt") {
    char* fake[1];
    fake[0] = (char*)"--palette-generation-algorithm=\"mixed\"";
    optReader.read(1, fake);
    REQUIRE(pga.value() == "mixed");
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

  SECTION("Combined shortopts") {
    char* fake[2];
    fake[0] = (char*)"-ba";
    fake[1] = (char*)"kmeans";
    optReader.read(2, fake);
    REQUIRE(pga.value() == "kmeans");
    REQUIRE(b.value() == true);
  }

  SECTION("Combined shortopts, argument using opts must be last") {
    char* fake[2];
    fake[0] = (char*)"-ab";
    fake[1] = (char*)"kmeans";
    bool failed = false;
    try {
      optReader.read(2, fake);
    } catch (const std::invalid_argument& e) {
      failed = true;
    }
    REQUIRE(failed == true);
  }

  SECTION("Shortopt with longopt style argument") {
    char* fake[1];
    fake[0] = (char*)"-f=\"quux\"";
    optReader.read(1, fake);
    REQUIRE(f.value() == "quux");
  }
}

TEST_CASE("Test skipping some arguments doesn't fail") {
  // This will work as "" will match "-f", i.e, there's and empty string in
  // "-f". This should be fixed with optional or with some optional-like hack,
  // ie using a pair<T, bool>. It could also be fixed by having a single
  // constructor instead of a fluent API... ;)
  SECTION("Missing superclass (Parameter) members") {
    String s = String();
    OptionReader optReader(vector<Parameter*>{&s});
    char* fake[2];
    fake[0] = (char*)"-f";
    fake[1] = (char*)"baz";
    optReader.read(2, fake);
    REQUIRE(s.value() == "baz");
  }

  SECTION("Missing superclass (Parameter) members") {
    Set set = Set().shortOpt('a').alternatives(vector<string>());
    OptionReader optReader(vector<Parameter*>{&set});
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
}