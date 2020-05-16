//
// Created by ANDREW SMITH on 16/05/2020.
//

#include "ASTValidator.h"
#include "fmt/core.h"

#include <utility>
til::ASTValidator::ASTValidator(std::shared_ptr<ErrorReporter> error_reporter, til::AST *ast)
    : error_reporter_(std::move(error_reporter)), ast_(ast) {

}

void til::ASTValidator::Validate() {
  ResolveMessages();
  ResolveAllServiceArgs();
}

void til::ASTValidator::ResolveMessages() {
  for (int i = 0; i < ast_->DeclarationCount(); i++) {
    if (ast_->Declaration(i)->t()!=Declaration::kMessage) {
      continue;
    }
    const auto *md = dynamic_cast<const MessageDeclaration *>(ast_->Declaration(i));
    ResolveMessageFields(md);
  }
}

void til::ASTValidator::ResolveMessageFields(const til::MessageDeclaration *md) {
  for (int i = 0; i < md->FieldCount(); i++) {
    ResolveTypeDef(md->Field(i)->type_def(), md->Field(i), md);
  }
}

void til::ASTValidator::ResolveTypeDef(const til::TypeDef *td, const Field *f, const MessageDeclaration *md) {
  switch (td->t()) {

    case TypeDef::kScalar:
      // Do nothing for scalar. It doesn't need resolving
      break;
    case TypeDef::kMessage:ResolveMessageTypeDef(dynamic_cast<const MessageTypeDef *>(td), f, md);
      break;
    case TypeDef::kMap:ResolveMapTypeDef(dynamic_cast<const MapTypeDef *>(td), f, md);
      break;
    case TypeDef::kList:ResolveListTypeDef(dynamic_cast<const ListTypeDef *>(td), f, md);
      break;
  }
}

void til::ASTValidator::ResolveMessageTypeDef(const til::MessageTypeDef *mtd,
                                              const Field *f,
                                              const MessageDeclaration *md) {
  auto resolved = ast_->ResolveMessage(mtd->name());
  if (!resolved.has_value()) {
    error_reporter_->ReportError(fmt::format("Field {} has unknown type {} in Message {} at line {} column {}",
                                             f->name(),
                                             mtd->name(),
                                             md->name(),
                                             md->start_token().line,
                                             md->start_token().col));
  }
}

void til::ASTValidator::ResolveListTypeDef(const til::ListTypeDef *ltd, const Field *f, const MessageDeclaration *md) {
  // Just recurse into ResolveTypeDef for the list subtype
  ResolveTypeDef(ltd->sub_type(), f, md);
}

void til::ASTValidator::ResolveMapTypeDef(const til::MapTypeDef *mtd, const Field *f, const MessageDeclaration *md) {
  // Just recurse into ResolveTypeDef for the map subtype
  ResolveTypeDef(mtd->sub_type(), f, md);
}

void til::ASTValidator::ResolveAllServiceArgs() {
  for (int i = 0; i < ast_->DeclarationCount(); i++) {
    if(ast_->Declaration(i)->t() != Declaration::kService) {
      continue;
    }
    ResolveServiceArgs(dynamic_cast<const ServiceDeclaration*>(ast_->Declaration(i)));
  }
}

void til::ASTValidator::ResolveServiceArgs(const til::ServiceDeclaration *sd) {
  for (int i = 0; i < sd->CallCount(); i++) {
    ResolveCallArguments(sd->Call(i), sd);
  }
}

void til::ASTValidator::ResolveCallArguments(const til::Call *call, const til::ServiceDeclaration *sd) {
  auto arg = ast_->ResolveMessage(call->argument().name());
  auto ret = ast_->ResolveMessage((call->argument().name()));
  if (!arg.has_value()) {
    error_reporter_->ReportError(fmt::format("Call {} has unknown argument type {} in Service {} at line {} column {}",
                                             call->name(),
                                             call->argument().name(),
                                             sd->name(),
                                             sd->start_token().line,
                                             sd->start_token().col));
  }
  if(!ret.has_value()) {
    error_reporter_->ReportError(fmt::format("Call {} has unknown return type {} in Service {} at line {} column {}",
                                             call->name(),
                                             call->returns().name(),
                                             sd->name(),
                                             sd->start_token().line,
                                             sd->start_token().col));
  }
}

