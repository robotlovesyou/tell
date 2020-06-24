#ifndef TELL_COMPILER_AST_H_
#define TELL_COMPILER_AST_H_

#include <map>
#include <optional>
#include <string>
#include <vector>
#include "Declaration.h"

namespace til {
struct SerializableAST; // forward declaration of SerializableAST

/**
 * Root AST class
 */
class AST {
 public:
  /**
   * Return the number of top level declarations
   * @return
   */
  [[nodiscard]] int DeclarationCount() const;

  /**
   * Add a new top level declaration
   * @param decl
   */
  void AddDeclaration(std::unique_ptr<Declaration> decl);

  /**
   * Return the top level declaration with the provided index
   * @param idx
   * @return
   */
  const til::Declaration *Declaration(int idx);

  /**
   * Find the delcaration index of the message with the provided name
   * @param name
   * @return
   */
  [[nodiscard]] std::optional<int> ResolveMessage(const std::string& name) const;

  /**
   * Convert the entire AST into a SerializableAST
   * @return
   */
  [[nodiscard]] SerializableAST ToSerializable() const;
 private:

  /**
   * Add the provided declaration to the provided index, mapped by name
   * @param decl
   * @param index
   * @param ord
   */
  void AddDeclToIndex(const til::Declaration *decl, std::map<std::string, int> *index, int ord);

  std::vector<std::unique_ptr<til::Declaration>> declarations_;
  std::map<std::string, int> directive_index_;
  std::map<std::string, int> message_index_;
  std::map<std::string, int> service_index_;
};
}

#endif //TELL_COMPILER_AST_H_
