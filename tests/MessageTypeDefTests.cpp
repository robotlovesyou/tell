#include "catch2/catch.hpp"
#include "MessageTypeDef.h"
#include "ParserTestHelpers.h"

TEST_CASE("MessageTypeDef constructor") {
  CHECK_NOTHROW(til::MessageTypeDef("MyMessage", test_ast(), false));
}

TEST_CASE("MessageTypeDef.optional()") {
  til::MessageTypeDef md("MyMessage", test_ast(), true);
  CHECK(md.optional());
}

TEST_CASE("MessageTypeDef.t()") {
  til::MessageTypeDef md("MyMessage", test_ast(), true);
  CHECK(md.t() == til::TypeDef::kMessage);
}

TEST_CASE("MessageTypeDef.name()") {
  til::MessageTypeDef md("MyMessage", test_ast(), true);
  CHECK(md.name() == "MyMessage");
}

