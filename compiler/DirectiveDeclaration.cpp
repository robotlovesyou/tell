//
// Created by ANDREW SMITH on 21/04/2020.
//

#include "DirectiveDeclaration.h"

#include <utility>
#include "fmt/core.h"

til::DirectiveDeclaration::DirectiveDeclaration(std::unique_ptr<til::Token> start_token,
                                                std::unique_ptr<til::DocCommentContext> doc,
                                                std::string name,
                                                std::string value,
                                                const std::shared_ptr<AST>& ast): start_token_(std::move(start_token)), doc_(std::move(doc)), name_(std::move(name)), value_(std::move(value)), ast_(ast) {
  if(start_token_->t != Token::kBang) {
    throw std::invalid_argument(fmt::format("Start token type cannot be a {}", start_token_->TypeName()));
  }

}

til::Declaration::Type til::DirectiveDeclaration::t() {
  return kDirective;
}

const std::string &til::DirectiveDeclaration::name() {
  return name_;
}

const til::AST &til::DirectiveDeclaration::ast() {
  return *ast_.lock();
}

const til::DocCommentContext &til::DirectiveDeclaration::doc() {
  return *doc_;
}

const std::string &til::DirectiveDeclaration::value() {
  return value_;
}

const til::Token &til::DirectiveDeclaration::start_token() {
  return *start_token_;
}
