#include "catch2/catch.hpp"

#include <utility>
#include "../compiler/Lexer.h"
#include "../compiler/StringCursor.h"
#include "../compiler/TestErrorReporter.h"

std::unique_ptr<til::Lexer> make_test_lexer(const std::string &source,
                                            const std::shared_ptr<til::ErrorReporter>& er = std::make_shared<til::TestErrorReporter>()) {
  auto sc = std::make_unique<til::StringCursor>(source);
  return std::make_unique<til::Lexer>(std::move(sc), er);
}

std::pair<std::string, til::Token::Type> pairs(const std::string &src, til::Token::Type t) {
  return std::pair<std::string, til::Token::Type>(src, t);
}

TEST_CASE("Lexer constructor") {
  auto sc = std::make_unique<til::StringCursor>("");
  auto er = std::make_shared<til::TestErrorReporter>();
  CHECK_NOTHROW(til::Lexer(std::move(sc), er));
}

TEST_CASE("Lexer.Next for single tokens. Happy path") {
  // Use a generator to generate the inputs for each test
  auto p = GENERATE(
      pairs("\"This is a string\"", til::Token::kString),
      pairs("/// This is a docstring", til::Token::kDocString),
      pairs("a_field_name", til::Token::kIdent),
      pairs("!", til::Token::kBang),
      pairs(":", til::Token::kColon),
      pairs("?", til::Token::kQMark),
      pairs("{", til::Token::kLBrace),
      pairs("}", til::Token::kRBrace),
      pairs("[", til::Token::kLSqBracket),
      pairs("]", til::Token::kRSqBracket),
      pairs("message", til::Token::kMessage),
      pairs("list", til::Token::kList),
      pairs("map", til::Token::kMap),
      pairs("service", til::Token::kService),
      pairs("float", til::Token::kFloat),
      pairs("int", til::Token::kInt),
      pairs("bool", til::Token::kBool),
      pairs("string", til::Token::kStringWord),
      pairs("time", til::Token::kTime),
      pairs("\n", til::Token::kLineFeed),
      pairs("", til::Token::kEOF)
  );


  // Create a token from the generated pair. Check the peek value, check the next value
  auto lx = make_test_lexer(std::get<0>(p));
  auto peeked = lx->Peek();
  CHECK(peeked);
  CHECK((**peeked).Literal()==std::get<0>(p));
  CHECK((**peeked).line==1);
  CHECK((**peeked).col==1);
  CHECK((**peeked).t==std::get<1>(p));
  auto nt = lx->Next();
  CHECK(nt);
  CHECK((*nt).Literal()==std::get<0>(p));
  CHECK((*nt).line==1);
  CHECK((*nt).col==1);
}

TEST_CASE("Lexer.Next string sad paths") {
  SECTION("Newline in string") {
    auto er = std::make_shared<til::TestErrorReporter>();
    auto lx = make_test_lexer("\"This is a \nbroken string", er);
    auto next = lx->Next();
    CHECK(er->has_errors());
    CHECK(next);
    CHECK((*next).t == til::Token::kEOF);
  }
  SECTION("EOF in string") {
    auto er = std::make_shared<til::TestErrorReporter>();
    auto lx = make_test_lexer("\"This is a broken string", er);
    auto next = lx->Next();
    CHECK(er->has_errors());
    CHECK(next);
    CHECK((*next).t == til::Token::kEOF);
  }
}

TEST_CASE("Lexer.Next tokens with whitespace") {
  std::string test_input = "\t   \t\r\r\r\t[  \t]";
  auto er = std::make_shared<til::TestErrorReporter>();
  auto lx = make_test_lexer(test_input);
  auto next = lx->Next();
  CHECK(next);
  CHECK((*next).t == til::Token::kLSqBracket);
  CHECK((*next).col == 10);
  CHECK((*next).line == 1);
  next = lx->Next();
  CHECK(next);
  CHECK((*next).t == til::Token::kRSqBracket);
  CHECK((*next).col == 14);
  CHECK((*next).line == 1);
  next = lx->Next();
  CHECK(next);
  CHECK((*next).t == til::Token::kEOF);
  CHECK((*next).col == 15);
  CHECK((*next).line == 1);
  CHECK_FALSE(er->has_errors());
}

TEST_CASE("Lexer.Next message definition") {
  std::string test_input =
      "message {\n\r\n"
      "\ta_field: int\n"
      "\tan_optional_field: int?\n"
      "}";

  auto er = std::make_shared<til::TestErrorReporter>();
  auto lx = make_test_lexer(test_input, er);
  std::vector<til::Token::Type> types = {
      til::Token::kMessage,
      til::Token::kLBrace,
      til::Token::kLineFeed,
      til::Token::kIdent,
      til::Token::kColon,
      til::Token::kInt,
      til::Token::kLineFeed,
      til::Token::kIdent,
      til::Token::kColon,
      til::Token::kInt,
      til::Token::kQMark,
      til::Token::kLineFeed,
      til::Token::kRBrace
  };
  for (auto t: types) {
    til::Token info{t, 0, 0, ""};
    INFO("expecting " << info.TypeName());
    auto next = lx->Next();
    CHECK(next);
    CHECK((*next).t == t);
  }
}