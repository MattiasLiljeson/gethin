#include <regex>
#include <string>
#include "ReHelper.hpp"
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

  SECTION("Flag with argument should fail") {
    char* fake[2];
    fake[0] = (char*)"--bar";
    fake[1] = (char*)"baz";
    bool failed = false;
    try {
      optReader.read(2, fake);
    } catch (const std::invalid_argument& e) {
      failed = true;
    }
    REQUIRE(failed == true);
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

// Mandatory option? yeah, we support that oxymoron!
TEST_CASE("Mandatory Flag") {
  Flag f = Flag().longOpt("bar").mandatory();
  OptionReader optReader({&f});
  char* fake[1];
  fake[0] = (char*)"--bar";
  optReader.read(1, fake);
  REQUIRE(f.value() == true);
}

// Mandatory option? yeah, we support that oxymoron!
TEST_CASE("Missing mandatory Flag") {
  Flag a = Flag().shortOpt('a');
  Flag f = Flag().longOpt("bar").mandatory();
  OptionReader optReader({&a, &f});
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

TEST_CASE("Windows style Flag options") {
  Flag a = Flag().shortOpt('a').longOpt("asd").help("some help text about asd");
  OptionReader optReader({&a});

  SECTION("Windows style Flag shortopt") {
    char* fake[]{(char*)"/a"};
    optReader.read(sizeof(fake) / sizeof(char*), fake);
    REQUIRE(a.value() == true);
  }

  SECTION("Windows style Flag longopt") {
    char* fake[]{(char*)"/asd"};
    optReader.read(sizeof(fake) / sizeof(char*), fake);
    REQUIRE(a.value() == true);
  }
}