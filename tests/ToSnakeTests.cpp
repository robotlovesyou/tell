#include "catch2/catch.hpp"

#include <string>
#include <tuple>

#include "Strcase.h"
#include "CaseTestHelpers.h"

/**
 * The following tests are ported from the strcase Go library
 * https://github.com/iancoleman/strcase
 */
TEST_CASE("ToSnake") {
  auto cases = GENERATE(
      case_test("testCase", "test_case"),
      case_test("TestCase", "test_case"),
      case_test("Test Case", "test_case"),
      case_test(" Test Case", "test_case"),
      case_test("Test Case ", "test_case"),
      case_test(" Test Case ", "test_case"),
      case_test("test", "test"),
      case_test("test_case", "test_case"),
      case_test("Test", "test"),
      case_test("", ""),
      case_test("ManyManyWords", "many_many_words"),
      case_test("manyManyWords", "many_many_words"),
      case_test("AnyKind of_string", "any_kind_of_string"),
      case_test("numbers2and55with000", "numbers_2_and_55_with_000"),
      case_test("JSONData", "json_data"),
      case_test("userID", "user_id"),
      case_test("AAAbbb", "aa_abbb")
  );

  auto before = std::get<0>(cases);
  auto after = std::get<1>(cases);

  CHECK(gen::ToSnake(before) == after);
}