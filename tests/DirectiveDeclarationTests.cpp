#include <utility>

#include "catch2/catch.hpp"

#include "../til/AST.h"
#include "../til/DirectiveDeclaration.h"
#include "ParserTestHelpers.h"

TEST_CASE("DirectiveDeclaration constructor") {
  CHECK_THROWS_AS(test_directive_declaration(test_token(til::Token::kRBrace)), std::invalid_argument);
}

TEST_CASE("DirectiveDeclaration.t()") {
  auto dd = test_directive_declaration();
  CHECK(dd->t() == til::Declaration::kDirective);
}

TEST_CASE("DirectiveDeclaration.name()") {
  std::string name = "a_name";
  auto dd = test_directive_declaration(test_token(til::Token::kBang), test_dcc(), name);
  CHECK(dd->name() == name);
}

TEST_CASE("DirectiveDeclaration.doc()") {
  auto doc = test_dcc();
  auto doc_ptr = doc.get();
  auto dd = test_directive_declaration(test_token(til::Token::kBang), std::move(doc));
  CHECK(&dd->doc() == doc_ptr);
}

TEST_CASE("DirectiveDeclaration.value()") {
  std::string value = "a value";
  auto dd = test_directive_declaration(test_token(til::Token::kBang), test_dcc(), "a_name", value);
  CHECK(dd->value() == value);
}

TEST_CASE("DirectiveDeclaration.ast()") {
  auto ast = test_ast();
  auto ast_ptr = ast.get();
  auto dd = test_directive_declaration(test_token(til::Token::kBang), test_dcc(), "a_name", "a value", ast);
  CHECK(&dd->ast() == ast_ptr);
}

TEST_CASE("DirectiveDeclaration.start_token()") {
  auto tk = test_token(til::Token::kBang);
  auto tk_ptr = tk.get();
  auto dd = test_directive_declaration(std::move(tk));
  CHECK(&dd->start_token() == tk_ptr);
}