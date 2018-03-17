#include "catch.hpp"
#include "gethin.hpp"

using gethin::OptionReader;
using gethin::String;

TEST_CASE("Test String option type") {
  String f = String()
                 .shortOpt('f')
                 .longOpt("foo")
                 .mandatory(false)
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

  // The value will in this case be an empty string. An improvement could be
  // to throw instead as this is clearly erranous input.
  SECTION("String without argument") {
    char* fake[1];
    fake[0] = (char*)"--foo";
    optReader.read(1, fake);
    REQUIRE(f.value() == "");
  }
}