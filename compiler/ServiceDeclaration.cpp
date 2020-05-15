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
                                            std::vector<std::unique_ptr<til::Call>> calls,
                                            const std::shared_ptr<AST> &ast)
    : start_token_(std::move(start_token)),
      name_(std::move(name)),
      doc_(std::move(doc)),
      calls_(std::move(calls)),
      ast_(ast) {
  if (start_token_->t!=Token::kService) {
    throw std::invalid_argument(fmt::format("ServiceDeclaration start token cannot be a {}", start_token_->TypeName()));
  }
}

int til::ServiceDeclaration::CallCount() const {
  return calls_.size();
}

const til::Call &til::ServiceDeclaration::Call(int idx) const {
  if (idx >= calls_.size()) {
    throw std::out_of_range("Invalid call index");
  }
  return *calls_[idx];
}

std::string til::ServiceDeclaration::t_name() const {
  return "service";
}
