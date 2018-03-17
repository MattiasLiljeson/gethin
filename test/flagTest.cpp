#include "catch.hpp"
#include "gethin.hpp"

using gethin::Flag;
using gethin::OptionReader;

TEST_CASE("Test Flag option type") {
  Flag b = Flag().shortOpt('b').longOpt("bar").help("some help text about bar");
  OptionReader optReader({&b});

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
}