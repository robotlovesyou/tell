//
// Created by ANDREW SMITH on 18/04/2020.
//

#include "MessageDeclaration.h"

#include <stdexcept>
#include <utility>

#include "fmt/core.h"

til::Declaration::Type til::MessageDeclaration::t() {
  return kMessage;
}

const std::string &til::MessageDeclaration::name() {
  return name_;
}

til::MessageDeclaration::MessageDeclaration(std::unique_ptr<til::Token> start_token, std::unique_ptr<til::DocCommentContext> doc, std::string name, const std::shared_ptr<AST> &ast)
    : start_token_(std::move(start_token)), doc_(std::move(doc)), name_(std::move(name)), ast_(ast) {

  if (start_token_->t!=Token::kMessage) {
    throw std::invalid_argument(fmt::format("MessageDeclaration start token cannot be a {}", start_token_->TypeName()));
  }
}

const til::DocCommentContext &til::MessageDeclaration::doc() {
  return *doc_;
}

const til::AST &til::MessageDeclaration::ast() {
  return *ast_.lock();
}

const til::Token &til::MessageDeclaration::start_token() {
  return *start_token_;
}
