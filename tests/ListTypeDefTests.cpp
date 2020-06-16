#include "catch2/catch.hpp"
#include "ParserTestHelpers.h"
#include "../til/ListTypeDef.h"

TEST_CASE("ListTypeDef constructor") {
  auto sd = std::make_unique<til::ScalarTypeDef>(til::ScalarTypeDef::kBool, false);
  CHECK_NOTHROW(til::ListTypeDef(std::move(sd), false));
}


TEST_CASE("ListTypeDef.optional()") {
  auto sd = std::make_unique<til::ScalarTypeDef>(til::ScalarTypeDef::kBool, false);
  til::ListTypeDef lt(std::move(sd), true);
  CHECK(lt.optional());
}

TEST_CASE("ListTypeDef.t()") {
  auto sd = std::make_unique<til::ScalarTypeDef>(til::ScalarTypeDef::kBool, false);
  til::ListTypeDef lt(std::move(sd), true);
  CHECK(lt.t() == til::TypeDef::kList);
}

TEST_CASE("ListTypeDef.sub_type()") {
  auto sd = std::make_unique<til::ScalarTypeDef>(til::ScalarTypeDef::kBool, false);
  til::ListTypeDef lt(std::move(sd), true);
  CHECK(lt.sub_type()->t() == til::TypeDef::kScalar);
}