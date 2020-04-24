#include "ServiceDeclaration.h"
#include "fmt/core.h"

#include <utility>
#include <iostream>
til::Declaration::Type til::ServiceDeclaration::t() const {
  return kService;
}

const til::Token &til::ServiceDeclaration::start_token() const {
  return *start_token_;
}

const std::string &til::ServiceDeclaration::name() const {
  return name_;
}

const til::DocCommentContext &til::ServiceDeclaration::doc() const {
  return *doc_;
}

const til::AST &til::ServiceDeclaration::ast() const {
  return *ast_.lock();
}

til::ServiceDeclaration::ServiceDeclaration(std::unique_ptr<Token> start_token,
                                            std::unique_ptr<DocCommentContext> doc,
                                            std::string name,
                                            const std::shared_ptr<AST>& ast): start_token_(std::move(start_token)), name_(std::move(name)), doc_(std::move(doc)), ast_(ast) {
  if (start_token_->t != Token::kService) {
    throw std::invalid_argument(fmt::format("ServiceDeclaration start token cannot be a {}", start_token_->TypeName()));
  }
}
