#include "catch2/catch.hpp"
#include "../compiler/AST.h"
#include "ParserTestHelpers.h"

TEST_CASE("AST.DeclarationCount()") {
  til::AST ast;
  SECTION("Empty AST") {
    CHECK(ast.DeclarationCount() == 0);
  }
  SECTION("With declarations") {
    ast.AddDeclaration(test_message_declaration());
    CHECK(ast.DeclarationCount() == 1);
  }
}

TEST_CASE("AST.AddDeclaration()") {
  til::AST ast;
  CHECK_NOTHROW(ast.AddDeclaration(test_directive_declaration()));
}

TEST_CASE("AST.Declaration()") {
  til::AST ast;
  std::string name = "my name";
  std::string doc = "This is the doc";
  auto dcc = std::make_unique<til::DocCommentContext>();
  dcc->append(doc);
  auto sd = test_service_declaration(test_token(til::Token::kService), std::move(dcc), name);

  ast.AddDeclaration(std::move(sd));
  SECTION("valid declaration") {
    auto decl = dynamic_cast<const til::ServiceDeclaration*>(ast.Declaration(0));
    CHECK(decl->t() == til::Declaration::kService);
    CHECK(decl->doc().content() == doc);
    CHECK(decl->name() == name);
    CHECK(decl->start_token().t == til::Token::kService);
  }

  SECTION("Invalid declaration") {
    CHECK_THROWS_AS(ast.Declaration(1), std::out_of_range);
  }
}