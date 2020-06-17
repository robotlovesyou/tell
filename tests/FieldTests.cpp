#include "catch2/catch.hpp"
#include "Field.h"
#include "ScalarTypeDef.h"
#include "ParserTestHelpers.h"

TEST_CASE("Field constructor") {
  CHECK_NOTHROW(test_field());
}

TEST_CASE("Field.name()") {
  auto tf = test_field();
  CHECK(tf.name() == "a_field");
}

TEST_CASE("Field.type_def()") {
  auto tf = test_field();
  CHECK_FALSE(tf.type_def()->optional());
  CHECK(tf.type_def()->t() == til::TypeDef::kScalar);
  CHECK((dynamic_cast<const til::ScalarTypeDef*>(tf.type_def()))->scalar_type() == til::ScalarTypeDef::kBool);
}

TEST_CASE("Field.doc()") {
  auto tf = test_field();
  CHECK_FALSE(tf.doc().content().empty());
}

