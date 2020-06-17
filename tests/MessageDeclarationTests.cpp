#include "catch2/catch.hpp"

#include "MessageDeclaration.h"
#include "AST.h"
#include "ParserTestHelpers.h"

TEST_CASE("MessageDeclaration constructor") {
  CHECK_THROWS_AS(test_message_declaration(test_token(til::Token::kQMark)), std::invalid_argument);
}

TEST_CASE("MessageDeclaration.t()") {
  auto md = test_message_declaration();
  CHECK(md->t()==til::Declaration::kMessage);
}

TEST_CASE("MessageDeclaration.name()") {
  std::string name = "SomeMessageName";
  auto md = test_message_declaration(test_token(til::Token::kMessage), test_dcc(), name);
  CHECK(md->name()==name);
}

TEST_CASE("MessageDeclaration.doc()") {
  std::string comment = "This is a comment";
  auto dcc = std::make_unique<til::DocCommentContext>();
  dcc->append(comment);
  auto md = test_message_declaration(test_token(til::Token::kMessage), std::move(dcc));
  CHECK(md->doc().content()==comment);
}

TEST_CASE("MessageDeclaration.ast()") {
  auto ast = std::make_shared<til::AST>();
  auto md = test_message_declaration(
      test_token(til::Token::kMessage),
      test_dcc(),
      "SomeName",
      test_empty_fields(),
      ast);

  CHECK(&md->ast()==ast.get());
}

TEST_CASE("MessageDeclaration.start_token()") {
  auto tk = test_token(til::Token::kMessage);
  auto tk_ptr = tk.get();
  auto md = test_message_declaration(std::move(tk));
  CHECK(&md->start_token()==tk_ptr);
}

TEST_CASE("MessageDeclaration.FieldCount()") {
  auto fields = test_empty_fields();
  fields.push_back(std::unique_ptr<til::Field>(new til::Field(test_field())));
  auto md = test_message_declaration(test_token(til::Token::kMessage), test_dcc(), "MyMessage", std::move(fields));
  CHECK(md->FieldCount() == 1);
}

TEST_CASE("MessageDeclaration.FieldEntry()") {
  auto fields = test_empty_fields();
  fields.push_back(std::unique_ptr<til::Field>(new til::Field(test_field())));
  auto md = test_message_declaration(test_token(til::Token::kMessage), test_dcc(), "MyMessage", std::move(fields));
  SECTION("Valid Entry") {
    CHECK_FALSE(md->Field(0)->name().empty());
  }

  SECTION("Invalid Entry") {
    CHECK_THROWS_AS(md->Field(1), std::out_of_range);
  }
}
