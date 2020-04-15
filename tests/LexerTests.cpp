#include "catch2/catch.hpp"

#include <utility>
#include "../compiler/Lexer.h"
#include "../compiler/StringCursor.h"

std::unique_ptr<til::Lexer> make_test_lexer(const std::string &source) {
  auto sc = std::make_unique<til::StringCursor>(source);
  auto er = std::make_shared<til::ErrorReporter>();
  return std::make_unique<til::Lexer>(std::move(sc), er);
}

std::pair<std::string, til::Token::Type> pairs(const std::string& src, til::Token::Type t) {
  return std::pair<std::string, til::Token::Type>(src, t);
}



TEST_CASE("Lexer constructor") {
  auto sc = std::make_unique<til::StringCursor>("");
  auto er = std::make_shared<til::ErrorReporter>();
  CHECK_NOTHROW(til::Lexer(std::move(sc), er));
}

TEST_CASE("Lexer.Next for single tokens. Happy path") {
  // Use a generator to generate the inputs for each test
  auto p = GENERATE(
      pairs("\"This is a string\"", til::Token::kString),
      pairs("/// This is a docstring", til::Token::kDocString),
      pairs("a_field_name", til::Token::kIdent)
      );

  // Create a token from the generated pair. Check the peek value, check the next value
  auto lx = make_test_lexer(std::get<0>(p));
  auto peeked = lx->Peek();
  CHECK(peeked);
  CHECK((**peeked).Literal() == std::get<0>(p));
  CHECK((**peeked).line == 1);
  CHECK((**peeked).col == 1);
  CHECK((**peeked).t == std::get<1>(p));
  auto nt = lx->Next();
  CHECK(nt);
  CHECK((*nt).Literal() == std::get<0>(p));
  CHECK((*nt).line == 1);
  CHECK((*nt).col == 1);

//  SECTION("Bang"){}
//  SECTION("Colon"){}
//  SECTION("QMark"){}
//  SECTION("LBrace"){}
//  SECTION("RBrace"){}
//  SECTION("LBracket"){}
//  SECTION("Message"){}
//  SECTION("List"){}
//  SECTION("Service"){}
//  SECTION("CALL"){}
//  SECTION("LineFeed"){}
//  SECTION("EOF"){}
}