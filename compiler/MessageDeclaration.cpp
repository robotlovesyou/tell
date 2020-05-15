//
// Created by ANDREW SMITH on 18/04/2020.
//

#include "MessageDeclaration.h"

#include <stdexcept>
#include <utility>

#include "fmt/core.h"
#include "ParsingException.h"

til::Declaration::Type til::MessageDeclaration::t() const {
  return kMessage;
}

const std::string &til::MessageDeclaration::name() const {
  return name_;
}

til::MessageDeclaration::MessageDeclaration(std::unique_ptr<til::Token> start_token,
                                            std::unique_ptr<til::DocCommentContext> doc,
                                            std::string name,
                                            std::vector<std::unique_ptr<til::Field>> fields,
                                            const std::shared_ptr<AST> &ast)
    : start_token_(std::move(start_token)),
      doc_(std::move(doc)),
      name_(std::move(name)),
      fields_(std::move(fields)),
      ast_(ast) {

  if (start_token_->t!=Token::kMessage) {
    throw std::invalid_argument(
        fmt::format("MessageDeclaration start token cannot be a {}", start_token_->TypeName()));
  }
  AddFieldsToIndex();
}

const til::DocCommentContext &til::MessageDeclaration::doc() const {
  return *doc_;
}

const til::AST &til::MessageDeclaration::ast() const {
  return *ast_.lock();
}

const til::Token &til::MessageDeclaration::start_token() const {
  return *start_token_;
}

int til::MessageDeclaration::FieldCount() const {
  return fields_.size();
}

const til::Field &til::MessageDeclaration::Field(int idx) const {
  if (idx >= fields_.size()) {
    throw std::out_of_range(fmt::format("invalid field entry {}", idx));
  }
  return *fields_[idx];
}

std::string til::MessageDeclaration::t_name() const {
  return "message";
}

void til::MessageDeclaration::AddFieldsToIndex() {
  for(int i = 0; i < fields_.size(); i++) {
    std::unique_ptr<til::Field> &field = fields_[i];

    if (field_index_.count(field->name())!=0) {
      throw ParsingException(fmt::format("Duplicate field name {} in message {} at line {} column {}",
                                         field->name(),
                                         this->name_,
                                         this->start_token_->line,
                                         this->start_token_->col));
    }

    field_index_[field->name()] = i;
  }
}
