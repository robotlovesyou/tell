//
// Created by ANDREW SMITH on 22/04/2020.
//

#ifndef TELL_TESTS_ASTTESTHELPERS_H_
#define TELL_TESTS_ASTTESTHELPERS_H_

#include "../compiler/AST.h"
#include "../compiler/DocCommentContext.h"
#include "../compiler/Token.h"

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

#endif //TELL_TESTS_ASTTESTHELPERS_H_
