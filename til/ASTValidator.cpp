#include "ASTValidator.h"
#include "fmt/core.h"

#include <algorithm>
#include <utility>
til::ASTValidator::ASTValidator(std::shared_ptr<ErrorReporter> error_reporter, til::AST *ast)
    : error_reporter_(std::move(error_reporter)), ast_(ast) {

}

void til::ASTValidator::Validate() {
  ResolveMessages();
  ResolveAllServiceArgs();
  if (!error_reporter_->has_errors()) {
    // This validation requires all message types to successfully resolve
    FindInfiniteRecursions();
  }
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
    if (ast_->Declaration(i)->t()!=Declaration::kService) {
      continue;
    }
    ResolveServiceArgs(dynamic_cast<const ServiceDeclaration *>(ast_->Declaration(i)));
  }
}

void til::ASTValidator::ResolveServiceArgs(const til::ServiceDeclaration *sd) {
  for (int i = 0; i < sd->CallCount(); i++) {
    ResolveCallArguments(sd->Call(i), sd);
  }
}

void til::ASTValidator::ResolveCallArguments(const til::Call *call, const til::ServiceDeclaration *sd) {
  auto arg = ast_->ResolveMessage(call->argument().name());
  auto ret = ast_->ResolveMessage((call->returns().name()));
  if (!arg.has_value()) {
    error_reporter_->ReportError(fmt::format("Call {} has unknown argument type {} in Service {} at line {} column {}",
                                             call->name(),
                                             call->argument().name(),
                                             sd->name(),
                                             sd->start_token().line,
                                             sd->start_token().col));
  }
  if (!ret.has_value()) {
    error_reporter_->ReportError(fmt::format("Call {} has unknown return type {} in Service {} at line {} column {}",
                                             call->name(),
                                             call->returns().name(),
                                             sd->name(),
                                             sd->start_token().line,
                                             sd->start_token().col));
  }
}

void til::ASTValidator::FindInfiniteRecursions() const {
  std::set<int> visited;
  std::vector<std::string> stack;

  try {
    for (int i = 0; i < ast_->DeclarationCount(); i++) {
      const auto *decl = ast_->Declaration(i);
      if (decl->t()!=Declaration::kMessage) {
        continue;
      }
      const auto *const msg_decl = dynamic_cast<const MessageDeclaration *>(decl);
      FindInfiniteRecursionsInMessage(msg_decl, &visited, &stack);
    }
  } catch (InfiniteRecursionError &e) {
    error_reporter_->ReportError(e.what());
  }

}

void til::ASTValidator::FindInfiniteRecursionsInMessage(const til::MessageDeclaration *md,
                                                        std::set<int> *visited,
                                                        std::vector<std::string> *stack) const {
  auto idx = ast_->ResolveMessage(md->name());
  // Assume idx has a value here because each message has already been shown to resolve
  // before this method is called.
  // If this message has already been visited then do not repeat the depth first search of it.
  if (visited->count(*idx)) {
    return;
  }
  // searching the stack like this is O(n^2) but the stacks are unlikely to ever be more than
  // a few items deep and this makes it simpler to display the loop in the error message
  auto it = std::find(stack->begin(), stack->end(), md->name());

  // if the current message appears in the stack then collect details of the recursion and throw
  // an infinite recursion error
  if (it!=stack->end()) {
    auto members = CollectRecursionMembers(it, stack, md->name());
    throw InfiniteRecursionError(members);
  }

  stack->push_back(md->name());

  for (int i = 0; i < md->FieldCount(); i++) {
    const auto *td = md->Field(i)->type_def();
    if (td->t()!=TypeDef::kMessage || td->optional()) {
      // Only non-optional Message type defs can be part of a recursion.
      // Scalars have no member fields, and the default for a map or a list is to have zero members
      continue;
    }
    const auto *mtd = dynamic_cast<const MessageTypeDef *>(td);
    auto msg_idx = ast_->ResolveMessage(mtd->name());
    // also safe to assume msg_idx has a value as described above
    const auto *child_md = dynamic_cast<const MessageDeclaration *>(ast_->Declaration(*msg_idx));
    FindInfiniteRecursionsInMessage(child_md, visited, stack);
  }

  stack->pop_back();
  visited->insert(*idx);
}

std::vector<std::string> til::ASTValidator::CollectRecursionMembers(std::vector<std::string>::iterator it,
                                                                    std::vector<std::string> *stack,
                                                                    const std::string &tail) const {
  std::vector<std::string> members;
  for (; it!=stack->end(); it++) {
    members.push_back(*it);
  }
  members.push_back(tail);
  return members;
}

til::ASTValidator::InfiniteRecursionError::InfiniteRecursionError(std::vector<std::string> members) {
  std::string path;
  for (int i = 0; i < members.size() - 1; i++) {
    path += members[i] + "->";
  }
  path += members[members.size() - 1];
  message_ =
      fmt::format("Infinitely recursive structure found: {}. Break the recursion by making a field type optional",
                  path);
}

const char *til::ASTValidator::InfiniteRecursionError::what() {
  return message_.c_str();
}
