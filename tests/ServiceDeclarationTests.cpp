#include "catch2/catch.hpp"

#include "../compiler/ServiceDeclaration.h"
#include "../compiler/AST.h"
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
  auto ast = test_ast();
  auto sd = test_service_declaration(test_token(til::Token::kService), test_dcc(), "a_name", ast);
  CHECK(&sd->ast() == ast.get());
}

TEST_CASE("ServiceDeclaration.start_token()") {
  auto tk = test_token(til::Token::kService);
  auto token_ptr = tk.get();
  auto sd = test_service_declaration(std::move(tk));
  CHECK(&sd->start_token() == token_ptr);
}