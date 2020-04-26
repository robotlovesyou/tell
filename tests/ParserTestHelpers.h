//
// Created by ANDREW SMITH on 22/04/2020.
//

#ifndef TELL_TESTS_PARSERTESTHELPERS_H_
#define TELL_TESTS_PARSERTESTHELPERS_H_

#include "../compiler/AST.h"
#include "../compiler/DocCommentContext.h"
#include "../compiler/Token.h"
#include "../compiler/DirectiveDeclaration.h"
#include "../compiler/MessageDeclaration.h"
#include "../compiler/ServiceDeclaration.h"

static std::unique_ptr<til::Token> test_token(til::Token::Type t) {
  return std::make_unique<til::Token>(t, 1, 1, "");
}

static std::unique_ptr<til::DocCommentContext> test_dcc() {
  auto dcc = std::make_unique<til::DocCommentContext>();
  dcc->append("A doc comment line");
  dcc->append("Another doc comment line");
  return dcc;
}

static std::shared_ptr<til::AST> test_ast() {
  return std::make_shared<til::AST>();
}

static std::unique_ptr<til::DirectiveDeclaration> test_directive_declaration(std::unique_ptr<til::Token> token = test_token(til::Token::kBang),
                                                     std::unique_ptr<til::DocCommentContext> dcc = test_dcc(),
                                                     const std::string &name = "directive_name",
                                                     const std::string &value = "directive_value",
                                                     const std::shared_ptr<til::AST> &ast = test_ast()) {
  return std::make_unique<til::DirectiveDeclaration>(std::move(token), std::move(dcc), name, value, ast);
}

static std::vector<std::unique_ptr<til::Field>> test_empty_fields() {
  return std::vector<std::unique_ptr<til::Field>>();
}

static std::unique_ptr<til::MessageDeclaration> test_message_declaration(std::unique_ptr<til::Token> tkn = test_token(til::Token::kMessage),
                                                 std::unique_ptr<til::DocCommentContext> dcc = test_dcc(),
                                                 const std::string &name = "MyMessage",
                                                 std::vector<std::unique_ptr<til::Field>> fields = test_empty_fields(),
                                                 const std::shared_ptr<til::AST> &ast = std::make_shared<til::AST>()) {
  return std::make_unique<til::MessageDeclaration>(std::move(tkn), std::move(dcc), name, std::move(fields), ast);
}

static std::unique_ptr<til::ServiceDeclaration> test_service_declaration(std::unique_ptr<til::Token> tkn = test_token(til::Token::kService),
                                                 std::unique_ptr<til::DocCommentContext> dcc = test_dcc(),
                                                 const std::string &name = "my_service",
                                                 const std::shared_ptr<til::AST> &ast = test_ast()) {
  return std::make_unique<til::ServiceDeclaration>(std::move(tkn), std::move(dcc), name, ast);
}

#endif //TELL_TESTS_PARSERTESTHELPERS_H_
