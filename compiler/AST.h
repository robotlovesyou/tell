//
// Created by ANDREW SMITH on 21/04/2020.
//

#ifndef TELL_COMPILER_AST_H_
#define TELL_COMPILER_AST_H_

#include <map>
#include <optional>
#include <string>
#include <vector>
#include "Declaration.h"

namespace til {
struct SerializableAST; // forward declaration of SerializableAST
class AST {
 public:
  [[nodiscard]] int DeclarationCount() const;
  void AddDeclaration(std::unique_ptr<Declaration> decl);
  const Declaration *Declaration(int idx);
  [[nodiscard]] std::optional<int> ResolveMessage(const std::string& name) const;
  [[nodiscard]] SerializableAST ToSerializable() const;
 private:
  void AddDeclToIndex(const til::Declaration *decl, std::map<std::string, int> *index, int ord);

  std::vector<std::unique_ptr<til::Declaration>> declarations_;
  std::map<std::string, int> directive_index_;
  std::map<std::string, int> message_index_;
  std::map<std::string, int> service_index_;
};
}

#endif //TELL_COMPILER_AST_H_
