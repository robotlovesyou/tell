#include "catch2/catch.hpp"

#include "../compiler/MessageDeclaration.h"
#include "../compiler/AST.h"

til::Token test_token() {
  return til::Token{til::Token::kMessage, 1, 1, til::kKeywordMessage};
}

til::DocCommentContext test_dcc() {
  til::DocCommentContext dcc;
  dcc += "A doc comment line";
  dcc += "Another doc comment line";
  return dcc;
}

til::MessageDeclaration test_message_declaration(til::Token tkn = test_token(), til::DocCommentContext dcc = test_dcc(), const std::string& name = "MyMessage", const std::shared_ptr<til::AST>& ast = std::make_shared<til::AST>()) {
  return til::MessageDeclaration(std::move(tkn), std::move(dcc), name, ast);
}

TEST_CASE("MessageDeclaration constructor") {
  CHECK_THROWS_AS(test_message_declaration(til::Token{til::Token::kQMark, 1, 1, "?"}), std::invalid_argument);
}

TEST_CASE("MessageDeclaration.t()") {
  auto md = test_message_declaration();
  CHECK(md.t() == til::Declaration::kMessage);
}

TEST_CASE("MessageDeclaration.name()") {
  std::string name = "SomeMessageName";
  auto md = test_message_declaration(test_token(), test_dcc(), name);
  CHECK(md.name() == name);
}

TEST_CASE("MessageDeclaration.doc()") {
  std::string comment = "This is a comment";
  til::DocCommentContext dcc;
  dcc += comment;
  auto md = test_message_declaration(test_token(), dcc);
  CHECK(*md.doc() == comment);
}

TEST_CASE("MessageDeclaration.ast()") {
  auto ast = std::make_shared<til::AST>();
  auto md = test_message_declaration(test_token(), test_dcc(), "SomeName", ast);
  CHECK(md.ast().get() == ast.get());
}
