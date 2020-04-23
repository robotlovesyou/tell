#include "catch2/catch.hpp"

#include "../compiler/MessageDeclaration.h"
#include "../compiler/AST.h"
#include "ASTTestHelpers.h"

til::MessageDeclaration test_message_declaration(std::unique_ptr<til::Token> tkn = test_token(til::Token::kMessage), std::unique_ptr<til::DocCommentContext> dcc = test_dcc(), const std::string& name = "MyMessage", const std::shared_ptr<til::AST>& ast = std::make_shared<til::AST>()) {
  return til::MessageDeclaration(std::move(tkn), std::move(dcc), name, ast);
}

TEST_CASE("MessageDeclaration constructor") {
  CHECK_THROWS_AS(test_message_declaration(test_token(til::Token::kQMark)), std::invalid_argument);
}

TEST_CASE("MessageDeclaration.t()") {
  auto md = test_message_declaration();
  CHECK(md.t() == til::Declaration::kMessage);
}

TEST_CASE("MessageDeclaration.name()") {
  std::string name = "SomeMessageName";
  auto md = test_message_declaration(test_token(til::Token::kMessage), test_dcc(), name);
  CHECK(md.name() == name);
}

TEST_CASE("MessageDeclaration.doc()") {
  std::string comment = "This is a comment";
  auto dcc = std::make_unique<til::DocCommentContext>();
  dcc->append(comment);
  auto dcc_ptr = dcc.get();
  auto md = test_message_declaration(test_token(til::Token::kMessage), std::move(dcc));
  CHECK(md.doc().content() == comment);
}

TEST_CASE("MessageDeclaration.ast()") {
  auto ast = std::make_shared<til::AST>();
  auto md = test_message_declaration(test_token(til::Token::kMessage), test_dcc(), "SomeName", ast);
  CHECK(&md.ast() == ast.get());
}

TEST_CASE("MessageDeclaration.start_token()") {
  auto tk = test_token(til::Token::kMessage);
  auto tk_ptr = tk.get();
  auto md = test_message_declaration(std::move(tk));
  CHECK(&md.start_token() == tk_ptr);
}
