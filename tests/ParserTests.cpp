#include "catch2/catch.hpp"
#include <tuple>
#include "../compiler/Parser.h"
#include "../compiler/StringCursor.h"
#include "../compiler/ConsoleErrorReporter.h"
#include "../compiler/Lexer.h"
#include "../compiler/TypeDef.h"
#include "ParserTestHelpers.h"


std::shared_ptr<til::ErrorReporter> test_error_reporter() {
  return std::make_shared<til::ConsoleErrorReporter>();
}

std::unique_ptr<til::Lexer> test_lexer(const char * source, const std::shared_ptr<til::ErrorReporter>& er) {
  auto cursor = std::make_unique<til::StringCursor>(source);
  return std::make_unique<til::Lexer>(std::move(cursor), er);
}

TEST_CASE("Parser constructor") {
  auto er = test_error_reporter();
  CHECK_NOTHROW(til::Parser(test_lexer("", er), er));
}

TEST_CASE("Parser.parse bad top level token") {
  FAIL("pending");
}

TEST_CASE("Parser.parse directive") {

  SECTION("Valid directive") {
    const char * source = R"SOURCE(
/// This is a comment about the directive
/// which is continued here
!a_directive "the directive value"
)SOURCE";

    auto er = test_error_reporter();
    auto tl = test_lexer(source, er);

    til::Parser p(std::move(tl), er);
    auto ast = p.Parse();
    CHECK(ast->DeclarationCount() == 1);
    CHECK(ast->Declaration(0)->t() == til::Declaration::kDirective);
    auto dd = dynamic_cast<const til::DirectiveDeclaration*>(ast->Declaration(0));
    CHECK(dd->doc().content() == "This is a comment about the directive\nwhich is continued here");
    CHECK(dd->name() == "a_directive");
    CHECK(dd->value() == "the directive value");
    CHECK_FALSE(er->has_errors());
  }

  SECTION("No ident") {
    const char *source=R"SOURCE(
!"the directive value"
)SOURCE";
    auto er = test_error_reporter();
    auto tl = test_lexer(source, er);
    til::Parser p(std::move(tl), er);
    auto ast = p.Parse();
    CHECK(ast->DeclarationCount() == 0);
    CHECK(er->has_errors());
  }

  SECTION("No value") {
    const char *source=R"SOURCE(
!a_directive
)SOURCE";

    auto er = test_error_reporter();
    auto tl = test_lexer(source, er);
    til::Parser p(std::move(tl), er);
    auto ast = p.Parse();
    CHECK(ast->DeclarationCount() == 0);
    CHECK(er->has_errors());
  }

  SECTION("No line feed after directive") {
    const char*source=R"SOURCE(
!a_directive "the directive value")SOURCE";

    auto er = test_error_reporter();
    auto tl = test_lexer(source, er);
    til::Parser p(std::move(tl), er);
    auto ast = p.Parse();
    CHECK(ast->DeclarationCount() == 0);
    CHECK(er->has_errors());
  }
}

std::tuple<int, bool, bool, std::string, til::ScalarTypeDef::ScalarType> scalar_test_opt(int idx, bool optional, bool has_docs, std::string name, til::ScalarTypeDef::ScalarType t) {
  return std::tuple<int, bool, bool, std::string, til::ScalarTypeDef::ScalarType>(idx, optional, has_docs, name, t);
}

TEST_CASE("Parser.parse message") {
  SECTION("Message with no fields") {
    const char *source=R"SOURCE(
message an_empty_message {
}
)SOURCE";

    auto er = test_error_reporter();
    auto tl = test_lexer(source, er);
    til::Parser p(std::move(tl), er);
    auto ast = p.Parse();
    CHECK_FALSE (er->has_errors());
    CHECK(ast->DeclarationCount() == 1);
    CHECK(ast->Declaration(0)->t() == til::Declaration::kMessage);
    auto md = dynamic_cast<const til::MessageDeclaration*>(ast->Declaration(0));
    CHECK(md->name() == "an_empty_message");
  }

  SECTION("Message with scalar fields") {
    const char *source=R"SOURCE(
message a_scalar_only_message {
  a_bool_field: bool

  /// A doc comment
  /// for an optional_bool_field
  an_optional_bool_field: bool?

  a_float_field: float
  an_optional_float_field: float?
  /// A doc comment for an_int_field
  an_int_field: int
  an_optional_int_field: int?
  a_string_field: string
  an_optional_string_field: string?
  /// A doc comment for a_time_field

  a_time_field: time
  an_optional_time_field: time?
}
)SOURCE";

    auto er = test_error_reporter();
    auto tl = test_lexer(source, er);
    til::Parser p(std::move(tl), er);
    auto ast = p.Parse();
    CHECK_FALSE(er->has_errors());
    CHECK(ast->DeclarationCount() == 1);
    CHECK(ast->Declaration(0)->t() == til::Declaration::kMessage);
    auto md = dynamic_cast<const til::MessageDeclaration*>(ast->Declaration(0));
    CHECK(md->name() == "a_scalar_only_message");
    CHECK(md->FieldCount() == 10);

    auto opts = GENERATE(
        scalar_test_opt(0, false, false, "a_bool_field", til::ScalarTypeDef::kBool),
        scalar_test_opt(1, true, true, "an_optional_bool_field", til::ScalarTypeDef::kBool),
        scalar_test_opt(2, false, false, "a_float_field", til::ScalarTypeDef::kFloat),
        scalar_test_opt(3, true, false, "an_optional_float_field", til::ScalarTypeDef::kFloat),
        scalar_test_opt(4, false, true, "an_int_field", til::ScalarTypeDef::kInt),
        scalar_test_opt(5, true, false, "an_optional_int_field", til::ScalarTypeDef::kInt),
        scalar_test_opt(6, false, false, "a_string_field", til::ScalarTypeDef::kString),
        scalar_test_opt(7, true, false, "an_optional_string_field", til::ScalarTypeDef::kString),
        scalar_test_opt(8, false, true, "a_time_field", til::ScalarTypeDef::kTime),
        scalar_test_opt(9, true, false, "an_optional_time_field", til::ScalarTypeDef::kTime)
        );

    auto idx = std::get<0>(opts);
    auto optional = std::get<1>(opts);
    auto has_docs = std::get<2>(opts);
    auto name = std::get<3>(opts);
    auto scalar_type = std::get<4>(opts);

    INFO(name);
    CHECK(md->FieldEntry(idx).type_def()->t() == til::TypeDef::kScalar);
    CHECK(md->FieldEntry(idx).type_def()->optional() == optional);
    CHECK(md->FieldEntry(idx).name() == name);
    CHECK(md->FieldEntry(idx).doc().has_content() == has_docs);
    auto std = dynamic_cast<const til::ScalarTypeDef*>(md->FieldEntry(idx).type_def());
    CHECK(std->scalar_type() == scalar_type);
  }
}

TEST_CASE("Parser.parse service") {
  FAIL("pending");
}

TEST_CASE("Parser.parse message with repeated field name") {
  FAIL("pending");
}

TEST_CASE("Parser.parse recursive message loop") {
  FAIL("pending");
}

TEST_CASE("Parser.parse duplicate message name") {
  FAIL("pending");
}

TEST_CASE("Parser.parse duplicate service name") {
  FAIL("pending");
}

TEST_CASE("Parser.parse unknown message as field type") {
  FAIL("pending");
}

TEST_CASE("Parser.parse unknown message as array type") {
  FAIL("pending");
}

TEST_CASE("Parser.parse unknown message as map type") {
  FAIL("pending");
}

TEST_CASE("Parser.parse unknown message as call argument") {
  FAIL("pending");
}

TEST_CASE("Parser.parse unknown message as call response") {
  FAIL("pending");
}

