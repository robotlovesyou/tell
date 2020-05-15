//
// Created by ANDREW SMITH on 21/04/2020.
//

#include "AST.h"
#include "fmt/core.h"
#include "ParsingException.h"

int til::AST::DeclarationCount() const {
  return declarations_.size();
}

void til::AST::AddDeclaration(std::unique_ptr<til::Declaration> decl) {
  std::map<std::string, int> *index = nullptr;
  switch (decl->t()) {
    case Declaration::kDirective:
      index = &directive_index_;
      break;
    case Declaration::kMessage:
      index = &message_index_;
      break;
    case Declaration::kService:
      index = &service_index_;
      break;
  }
  AddDeclToIndex(decl.get(), index, declarations_.size());
  declarations_.push_back(std::move(decl));
}

const til::Declaration *til::AST::Declaration(int idx) {
  if (idx >= declarations_.size()) {
    throw std::out_of_range("Declaration index out of range");
  }
  return declarations_[idx].get();
}

void til::AST::AddDeclToIndex(const til::Declaration *decl, std::map<std::string, int> *index, int ord) {
  if (index->count(decl->name())!=0) {
    throw til::ParsingException(fmt::format("Duplicate {} {} at line {} column {}",
                                            decl->t_name(),
                                            decl->name(),
                                            decl->start_token().line,
                                            decl->start_token().col));
  }
  (*index)[decl->name()] = ord;
}
