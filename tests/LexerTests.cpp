#include "catch2/catch.hpp"

#include "../compiler/Lexer.h"
#include "../compiler/StringCursor.h"

std::unique_ptr<til::Lexer> make_test_lexer(const std::string &source) {
  auto sc = std::make_unique<til::StringCursor>(source);
  auto er = std::make_shared<til::ErrorReporter>();
  return std::make_unique<til::Lexer>(std::move(sc), er);
}

TEST_CASE("Lexer constructor") {
  auto sc = std::make_unique<til::StringCursor>("");
  auto er = std::make_shared<til::ErrorReporter>();
  CHECK_NOTHROW(til::Lexer(std::move(sc), er));
}

TEST_CASE("Lexer.Next for single tokens") {
  SECTION("String"){}
  SECTION("DocString"){}
  SECTION("Ident"){}
  SECTION("Bang"){}
  SECTION("Colon"){}
  SECTION("QMark"){}
  SECTION("LBrace"){}
  SECTION("RBrace"){}
  SECTION("LBracket"){}
  SECTION("Message"){}
  SECTION("List"){}
  SECTION("Service"){}
  SECTION("CALL"){}
  SECTION("LineFeed"){}
  SECTION("EOF"){}
}