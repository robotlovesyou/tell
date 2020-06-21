//
// Created by ANDREW SMITH on 18/04/2020.
//

#ifndef TELL_COMPILER_MESSAGEDECLARATION_H_
#define TELL_COMPILER_MESSAGEDECLARATION_H_

#include <optional>
#include <map>
#include <string>
#include <vector>

#include "Declaration.h"
#include "Token.h"
#include "DocCommentContext.h"
#include "Field.h"

namespace til {
struct SerializableMessageDeclaration; // forward declaration of SerializableMessageDeclaration

/**
 * Represents a message top level declaration in the AST
 */
class MessageDeclaration : public Declaration {
 public:
  MessageDeclaration(std::unique_ptr<Token> start_token,
                     std::unique_ptr<DocCommentContext> doc,
                     std::string name,
                     std::vector<std::unique_ptr<Field>> fields,
                     const std::shared_ptr<AST> &ast);

  // Disallow copy construction and assignment. Allow move.
  ~MessageDeclaration() override = default;
  MessageDeclaration(const MessageDeclaration &) = delete;
  MessageDeclaration &operator=(const MessageDeclaration &) = delete;
  MessageDeclaration(MessageDeclaration &&) = default;
  MessageDeclaration &operator=(MessageDeclaration &&) = default;

  /**
   * Getters
   */
  [[nodiscard]] Type t() const override;
  [[nodiscard]] const std::string &name() const override;
  [[nodiscard]] const DocCommentContext &doc() const override;
  [[nodiscard]] const AST &ast() const override;
  [[nodiscard]] const Token &start_token() const override;
  [[nodiscard]] std::string t_name() const override;

  /**
   * Return the field count of the message
   * @return
   */
  [[nodiscard]] int FieldCount() const;

  /**
   * Return the field with the provided index
   * @param idx
   * @return
   */
  [[nodiscard]] const Field *Field(int idx) const;

  /**
   * Convert the entire message structure into a SerializableMessage
   * @return
   */
  [[nodiscard]] std::unique_ptr<SerializableMessageDeclaration> ToSerializable() const;
 private:

  /**
   * Add each field to an index of fields and ensure each field name is unique within the message
   */
  void AddFieldsToIndex();

  std::unique_ptr<Token> start_token_;
  std::string name_;
  std::unique_ptr<DocCommentContext> doc_;
  std::vector<std::unique_ptr<til::Field>> fields_;
  std::map<std::string, int> field_index_;
  std::weak_ptr<AST> ast_;
};
}

#endif //TELL_COMPILER_MESSAGEDECLARATION_H_
