#include <string>
#include "gethin.hpp"
#include "catch.hpp"

using std::string;
using gethin::Optional;

TEST_CASE("Given empty constructor, return isPresent=>false") {
  Optional<string> foo;
  REQUIRE(!foo.isPresent());
}

TEST_CASE("Given string supplied to constructor, return isPresent=>true") {
  Optional<string> foo("bar");
  REQUIRE(foo.isPresent());
  REQUIRE(foo.get() == "bar");
}