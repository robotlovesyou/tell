#include "SerializableAST.h"

static const char * K_MESSAGE_DECLARATIONS = "message_declarations";
static const char * K_DIRECTIVE_DECLARATIONS = "directive_declarations";

void til::to_json(json &j, const SerializableAST &ast) {
  for (const auto & dir : ast.directive_declarations) {
    j[K_DIRECTIVE_DECLARATIONS].push_back(*dir);
  }

  for (const auto & msg : ast.message_declarations) {
    j[K_MESSAGE_DECLARATIONS].push_back(*msg);
  }
}

void til::from_json(const json &j, SerializableAST &ast) {
  for (const auto &element : j.at(K_DIRECTIVE_DECLARATIONS)) {
    auto decl = element.get<SerializableDirectiveDeclaration>();
    auto unique_decl = std::make_unique<SerializableDirectiveDeclaration>();
    *unique_decl = std::move(decl);
    ast.directive_declarations.push_back(std::move(unique_decl));
  }

  for (const auto &element : j.at(K_MESSAGE_DECLARATIONS)) {
    auto msg = element.get<SerializableMessageDeclaration>();
    auto unique_msg = std::make_unique<SerializableMessageDeclaration>();
    *unique_msg = std::move(msg);
    ast.message_declarations.push_back(std::move(unique_msg));
  }
}
