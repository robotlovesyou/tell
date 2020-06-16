#include "catch2/catch.hpp"

#include "../til/ServiceDeclaration.h"
#include "../til/AST.h"
#include "ParserTestHelpers.h"

TEST_CASE("ServiceDeclaration constructor") {
  CHECK_THROWS_AS(test_service_declaration(test_token(til::Token::kRBrace)), std::invalid_argument);
}

TEST_CASE("ServiceDeclaration.t()") {
  auto sd = test_service_declaration();
  CHECK(sd->t() == til::Declaration::kService);
}

TEST_CASE("ServiceDeclaration.name()") {
  std::string name = "SomeService";
  auto sd = test_service_declaration(test_token(til::Token::kService), test_dcc(), name);
  CHECK(sd->name() == name);
}

TEST_CASE("ServiceDeclaration.doc") {
  auto doc = test_dcc();
  auto doc_ptr = doc.get();
  auto sd = test_service_declaration(test_token(til::Token::kService), std::move(doc));
  CHECK(&sd->doc() == doc_ptr);
}

TEST_CASE("ServiceDeclaration.ast()") {
  std::vector<std::unique_ptr<til::Call>> calls;
  auto ast = test_ast();
  auto sd = test_service_declaration(test_token(til::Token::kService), test_dcc(), "a_name", std::move(calls), ast);
  CHECK(&sd->ast() == ast.get());
}

TEST_CASE("ServiceDeclaration.start_token()") {
  auto tk = test_token(til::Token::kService);
  auto token_ptr = tk.get();
  auto sd = test_service_declaration(std::move(tk));
  CHECK(&sd->start_token() == token_ptr);
}

TEST_CASE("ServiceDeclaration.CallCount()") {
  auto ast = test_ast();
  auto argument = std::make_unique<til::Argument>("my_argument", ast);
  auto returns = std::make_unique<til::Argument>("my_return", ast);
  auto doc = std::make_unique<til::DocCommentContext>();
  doc->append("A comment");
  auto call = std::make_unique<til::Call>("call_name", std::move(doc), std::move(argument), std::move(returns));
  std::vector<std::unique_ptr<til::Call>> calls;
  calls.push_back(std::move(call));
  auto sd = test_service_declaration(test_token(til::Token::kService), test_dcc(), "a_name", std::move(calls), ast);
  CHECK(sd->CallCount() == 1);
}

TEST_CASE("ServiceDeclaration.Call()") {
  SECTION("Valid idx") {
    auto ast = test_ast();
    auto argument = std::make_unique<til::Argument>("my_argument", ast);
    auto returns = std::make_unique<til::Argument>("my_return", ast);
    auto doc = std::make_unique<til::DocCommentContext>();
    doc->append("A comment");
    auto call = std::make_unique<til::Call>("call_name", std::move(doc), std::move(argument), std::move(returns));
    std::vector<std::unique_ptr<til::Call>> calls;
    calls.push_back(std::move(call));
    auto sd = test_service_declaration(test_token(til::Token::kService), test_dcc(), "a_name", std::move(calls), ast);
    CHECK(sd->Call(0)->name() == "call_name");
  }

  SECTION("Invalid idx") {
    auto sd = test_service_declaration();
    CHECK_THROWS_AS(sd->Call(0), std::out_of_range);
  }
}