#include "catch2/catch.hpp"
#include "../compiler/Parser.h"
#include "../compiler/StringCursor.h"
#include "../compiler/ConsoleErrorReporter.h"
#include "../compiler/Lexer.h"
#include "ParserTestHelpers.h"


std::shared_ptr<til::ErrorReporter> test_error_reporter() {
  return std::make_shared<til::ConsoleErrorReporter>();
}

std::unique_ptr<til::Lexer> test_lexer(const char * source, std::shared_ptr<til::ErrorReporter> er) {
  auto cursor = std::make_unique<til::StringCursor>(source);
  return std::make_unique<til::Lexer>(std::move(cursor), er);
}

TEST_CASE("Parser constructor") {
  auto er = test_error_reporter();
  CHECK_NOTHROW(til::Parser(test_lexer("", er), er));
}

TEST_CASE("Parser.parse directive") {
  const char * source = R"SOURCE(
!a_directive "the directive value"
)SOURCE";

  auto er = test_error_reporter();
  auto tl = test_lexer(source, er);

  til::Parser p(std::move(tl), er);
  auto ast = p.Parse();
  CHECK(ast->DeclarationCount() == 1);
  CHECK(ast->Declaration(0)->t() == til::Declaration::kDirective);
  auto dd = dynamic_cast<const til::DirectiveDeclaration*>(ast->Declaration(0));
  CHECK(dd->name() == "a_directive");
  CHECK(dd->value() == "the directive value");
  CHECK_FALSE(er->has_errors());
}

TEST_CASE("Parser.parse message") {
  FAIL("pending");
}

TEST_CASE("Parser.parse service") {
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

