//
// Created by ANDREW SMITH on 21/04/2020.
//

#include "AST.h"
#include "fmt/core.h"
#include "ParsingException.h"

#include "SerializableAST.h"
#include "DirectiveDeclaration.h"
#include "MessageDeclaration.h"
#include "ServiceDeclaration.h"
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

std::optional<int> til::AST::ResolveMessage(const std::string& name) const {
  if (message_index_.count(name) == 1) {
    return std::optional<int>(message_index_.at(name));
  }
  return std::optional<int>();
}

til::SerializableAST til::AST::ToSerializable() const {
  std::vector<std::unique_ptr<SerializableDirectiveDeclaration>> sdirs;
  std::vector<std::unique_ptr<SerializableMessageDeclaration>> smsgs;
  std::vector<std::unique_ptr<SerializableServiceDeclaration>> ssvcs;

  const DirectiveDeclaration *dir = nullptr;
  const MessageDeclaration *md = nullptr;
  const ServiceDeclaration *sd = nullptr;

  for (auto const & decl : declarations_) {
    switch (decl->t()) {
      case Declaration::kDirective:
        dir = dynamic_cast<const DirectiveDeclaration *>(decl.get());
        sdirs.push_back(std::move(dir->ToSerializable()));
        break;
      case Declaration::kMessage:
        md = dynamic_cast<const MessageDeclaration *>(decl.get());
        smsgs.push_back(std::move(md->ToSerializable()));
        break;
      case Declaration::kService:
        sd = dynamic_cast<const ServiceDeclaration *>(decl.get());
        ssvcs.push_back(std::move(sd->ToSerializable()));
        break;
    }
  }

  return SerializableAST(std::move(sdirs), std::move(smsgs), std::move(ssvcs));
}
