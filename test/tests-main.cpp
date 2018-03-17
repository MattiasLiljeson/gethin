#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "gethin.hpp"

using gethin::Flag;
using gethin::OptionReader;
using gethin::String;

TEST_CASE("Test combined shortOpts") {
  Flag a = Flag().shortOpt('a').longOpt("asd").help("some help text about asd");
  Flag b = Flag().shortOpt('b').longOpt("bar").help("some help text about bar");
  Flag c = Flag().shortOpt('c').longOpt("corge").help("help text about corge");
  String e = String().shortOpt('e').longOpt("ert").help("help text about foo");
  String f = String().shortOpt('f').longOpt("foo").help("help text about foo");

  OptionReader optReader({&a, &b, &c, &e, &f});

  SECTION("Combined shortopts") {
    char* fake[2];
    fake[0] = (char*)"-abcf";
    fake[1] = (char*)"baz";
    optReader.read(2, fake);
    REQUIRE(f.value() == "baz");
    REQUIRE(b.value() == true);
  }

  SECTION("Combined shortopts, argument using opts must be last") {
    char* fake[2];
    fake[0] = (char*)"-fabc";
    fake[1] = (char*)"quux";
    bool failed = false;
    try {
      optReader.read(2, fake);
    } catch (const std::invalid_argument& e) {
      failed = true;
    }
    REQUIRE(failed == true);
  }

  SECTION("Combined longopts, argument using opts must be last") {
    char* fake[2];
    fake[0] = (char*)"--asd";
    fake[1] = (char*)"--bar";
    fake[2] = (char*)"--corge";
    fake[3] = (char*)"--foo";
    fake[4] = (char*)"quux";
    optReader.read(5, fake);
    REQUIRE(f.value() == "quux");
  }

  SECTION("Shortopt with longopt style argument") {
    char* fake[1];
    fake[0] = (char*)"-f=\"corge\"";
    optReader.read(1, fake);
    REQUIRE(f.value() == "corge");
  }
}

TEST_CASE("Test not specifying all params") {
  SECTION("Not specifying anything") {
    String s = String();
    OptionReader optReader({&s});
    char* fake[2];
    fake[0] = (char*)"-f";
    fake[1] = (char*)"foo";
    bool failed = false;
    try {
      optReader.read(2, fake);
    } catch (const std::invalid_argument& e) {
      failed = true;
    }
    REQUIRE(failed == true);
  }

  SECTION("Only shortopt set") {
    String s = String().shortOpt('a');
    OptionReader optReader({&s});
    char* fake[2];
    fake[0] = (char*)"-a";
    fake[1] = (char*)"asd";
    optReader.read(2, fake);
    REQUIRE(s.value() == "asd");
  }

  SECTION("Only longopt set") {
    String s = String().longOpt("bar");
    OptionReader optReader({&s});
    char* fake[2];
    fake[0] = (char*)"--bar";
    fake[1] = (char*)"baz";
    optReader.read(2, fake);
    REQUIRE(s.value() == "baz");
  }
}

TEST_CASE("Test usage output") {
  Flag a = Flag().shortOpt('a').longOpt("asd").help("some help text about asd");
  Flag b = Flag().shortOpt('b').longOpt("bar").help("some help text about bar");
  Flag c = Flag().shortOpt('c').longOpt("corge").help("help text about corge");
  std::ostringstream os;
  OptionReader optReader({&a, &b, &c}, os);
  char* fake[1];
  fake[0] = (char*)"-h";
  optReader.read(1, fake);
  std::string out = os.str();
  REQUIRE(!out.empty());
}