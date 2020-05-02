#include "catch2/catch.hpp"
#include "../compiler/MessageTypeDef.h"
#include "ParserTestHelpers.h"
#include "../compiler/MapTypeDef.h"

TEST_CASE("MapTypeDef constructor") {
  auto st = std::make_unique<til::ScalarTypeDef>(til::ScalarTypeDef::kBool, false);
  til::MapTypeDef mtd(std::move(st), false);
}

TEST_CASE("MapTypeDef.optional()") {
  auto st = std::make_unique<til::ScalarTypeDef>(til::ScalarTypeDef::kBool, false);
  til::MapTypeDef mtd(std::move(st), true);
  CHECK(mtd.optional());
}

TEST_CASE("MapTypeDef.t()") {
  auto st = std::make_unique<til::ScalarTypeDef>(til::ScalarTypeDef::kBool, false);
  til::MapTypeDef mtd(std::move(st), true);
  CHECK(mtd.t() == til::TypeDef::kMap);
}

TEST_CASE("MapTypeDef.sub_type") {
  auto st = std::make_unique<til::ScalarTypeDef>(til::ScalarTypeDef::kBool, true);
  til::MapTypeDef mtd(std::move(st), true);
  CHECK(mtd.sub_type()->optional());
  CHECK(mtd.sub_type()->t() == til::TypeDef::kScalar);
}