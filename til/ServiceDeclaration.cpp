#include "ServiceDeclaration.h"
#include "ParsingException.h"
#include "fmt/core.h"

#include <utility>
#include <iostream>

#include "SerializableServiceDeclaration.h"

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
  AddCallsToIndex();
}

int til::ServiceDeclaration::CallCount() const {
  return calls_.size();
}

const til::Call *til::ServiceDeclaration::Call(int idx) const {
  if (idx >= calls_.size()) {
    throw std::out_of_range("Invalid call index");
  }
  return calls_[idx].get();
}

std::string til::ServiceDeclaration::t_name() const {
  return "service";
}

void til::ServiceDeclaration::AddCallsToIndex() {
  for(int i = 0; i < calls_.size(); i++) {
    std::unique_ptr<til::Call> &call = calls_[i];
    if (call_index_.count(call->name()) != 0) {
      throw ParsingException(fmt::format("Duplicate call name {} in service {} at line {} column {}",
                                         call->name(),
                                         this->name_,
                                         this->start_token_->line,
                                         this->start_token_->col));
    }
    call_index_[call->name()] = i;
  }
}

std::unique_ptr<til::SerializableServiceDeclaration> til::ServiceDeclaration::ToSerializable() const {
  std::vector<std::unique_ptr<SerializableCall>> scalls;
  for (const auto & call : calls_) {
    scalls.push_back(std::move(call->ToSerializable()));
  }

  return std::make_unique<SerializableServiceDeclaration>(name_, doc_->content(), std::move(scalls));
}
