//
// Created by ANDREW SMITH on 21/04/2020.
//

#include "AST.h"
int til::AST::DeclarationCount() const {
  return declarations_.size();
}

void til::AST::AddDeclaration(std::unique_ptr<til::Declaration> decl) {
  declarations_.push_back(std::move(decl));
}

const til::Declaration *til::AST::Declaration(int idx) {
  if (idx >= declarations_.size()) {
    throw std::out_of_range("Declaration index out of range");
  }
  return declarations_[idx].get();
}
