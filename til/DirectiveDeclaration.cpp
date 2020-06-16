//
// Created by ANDREW SMITH on 21/04/2020.
//

#include "DirectiveDeclaration.h"

#include <utility>
#include "fmt/core.h"

#include "SerializableDirectiveDeclaration.h"

til::DirectiveDeclaration::DirectiveDeclaration(std::unique_ptr<til::Token> start_token,
                                                std::unique_ptr<til::DocCommentContext> doc,
                                                std::string name,
                                                std::string value,
                                                const std::shared_ptr<AST>& ast): start_token_(std::move(start_token)), doc_(std::move(doc)), name_(std::move(name)), value_(std::move(value)), ast_(ast) {
  if(start_token_->t != Token::kBang) {
    throw std::invalid_argument(fmt::format("Start token type cannot be a {}", start_token_->TypeName()));
  }

}

til::Declaration::Type til::DirectiveDeclaration::t() const {
  return kDirective;
}

const std::string &til::DirectiveDeclaration::name() const {
  return name_;
}

const til::AST &til::DirectiveDeclaration::ast() const {
  return *ast_.lock();
}

const til::DocCommentContext &til::DirectiveDeclaration::doc() const {
  return *doc_;
}

const std::string &til::DirectiveDeclaration::value() const {
  return value_;
}

const til::Token &til::DirectiveDeclaration::start_token() const {
  return *start_token_;
}

std::string til::DirectiveDeclaration::t_name() const {
  return "directive";
}

std::unique_ptr<til::SerializableDirectiveDeclaration> til::DirectiveDeclaration::ToSerializable() const{
  return std::make_unique<SerializableDirectiveDeclaration>(name_, doc_->content(), value_);
}
