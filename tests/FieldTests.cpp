#include "catch2/catch.hpp"
#include "../compiler/Field.h"
#include "../compiler/ScalarTypeDef.h"
#include "ParserTestHelpers.h"

std::unique_ptr<til::TypeDef> test_td() {
  return std::make_unique<til::ScalarTypeDef>(til::ScalarTypeDef::kBool, false);
}

til::Field test_field(const std::string &name = "a_field",
                      std::unique_ptr<til::TypeDef> type_def = test_td(),
                      std::unique_ptr<til::DocCommentContext> doc = test_dcc()) {
  return til::Field(name, std::move(type_def), std::move(doc));
}

TEST_CASE("Field constructor") {
  CHECK_NOTHROW(test_field());
}

TEST_CASE("Field.name()") {
  auto tf = test_field();
  CHECK(tf.name() == "a_field");
}

TEST_CASE("Field.type_def()") {
  auto tf = test_field();
  CHECK_FALSE(tf.type_def().optional());
  CHECK(tf.type_def().t() == til::TypeDef::kScalar);
  CHECK((dynamic_cast<const til::ScalarTypeDef&>(tf.type_def())).scalar_type() == til::ScalarTypeDef::kBool);
}

TEST_CASE("Field.doc()") {
  auto tf = test_field();
  CHECK_FALSE(tf.doc().content().empty());
}

