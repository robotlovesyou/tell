#include "catch2/catch.hpp"
#include <optional>
#include <string>
#include "Strcase.h"
#include "CaseTestHelpers.h"

/**
 * The following tests are ported from the strcase Go library
 * https://github.com/iancoleman/strcase
 */
TEST_CASE("ToCamel tests") {
  auto camel_opts = GENERATE(
      case_test("test_case", "TestCase"),
      case_test("test.case", "TestCase"),
      case_test("test", "Test"),
      case_test("TestCase", "TestCase"),
      case_test(" test  case ", "TestCase"),
      case_test("", ""),
      case_test("many_many_words", "ManyManyWords"),
      case_test("AnyKind of_string", "AnyKindOfString"),
      case_test("odd-fix", "OddFix"),
      case_test("numbers2And55with000", "Numbers2And55With000"),
      case_test("ID", "Id")
  );
  auto camel_before = std::get<0>(camel_opts);
  auto camel_after = std::get<1>(camel_opts);
  CHECK(gen::ToCamel(camel_before)==camel_after);

  auto lower_camel_opts = GENERATE(
      case_test("foo-bar", "fooBar"),
      case_test("TestCase", "testCase"),
      case_test("", ""),
      case_test("AnyKind of_string", "anyKindOfString"),
      case_test("AnyKind.of-string", "anyKindOfString"),
      case_test("ID", "id")
  );

  auto lower_camel_before = std::get<0>(lower_camel_opts);
  auto lower_camel_after = std::get<1>(lower_camel_opts);
  CHECK(gen::ToCamel(lower_camel_before, false) == lower_camel_after);
}
