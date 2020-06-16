#include "catch2/catch.hpp"
#include "../til/ScalarTypeDef.h"

TEST_CASE("ScalarTypeDef constructor") {
  CHECK_NOTHROW(til::ScalarTypeDef(til::ScalarTypeDef::kString, false));
}

TEST_CASE("ScalarTypeDef.optional()") {
  til::ScalarTypeDef std(til::ScalarTypeDef::kInt, true);
  CHECK(std.optional());
}

TEST_CASE("ScalarTypeDef.t()") {
  til::ScalarTypeDef std(til::ScalarTypeDef::kFloat, false);
  CHECK(std.t() == til::TypeDef::kScalar);
}

TEST_CASE("ScalarTypeDef.scalar_type()") {
  til::ScalarTypeDef std(til::ScalarTypeDef::kTime, false);
  CHECK(std.scalar_type() == til::ScalarTypeDef::kTime);
}