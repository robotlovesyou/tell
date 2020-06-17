#include "catch2/catch.hpp"
#include "Argument.h"
#include "ParserTestHelpers.h"

TEST_CASE("Argument constructor") {
  auto ast = test_ast();
  CHECK_NOTHROW(til::Argument("a_message", ast));
}

TEST_CASE("Argument.name") {
  auto ast = test_ast();
  til::Argument arg("a_message", ast);
  CHECK(arg.name() == "a_message");
}