//
// Created by ANDREW SMITH on 18/04/2020.
//

#ifndef TELL_COMPILER_MESSAGEDECLARATION_H_
#define TELL_COMPILER_MESSAGEDECLARATION_H_

#include <optional>
#include <string>
#include "Declaration.h"
#include "Token.h"
#include "DocCommentContext.h"

namespace til {
class MessageDeclaration: public Declaration {
 public:
  MessageDeclaration(std::unique_ptr<Token> start_token, std::unique_ptr<DocCommentContext> doc, std::string name, const std::shared_ptr<AST> &ast);

  // Disallow copy construction and assignment. Allow move.
  ~MessageDeclaration() = default;
  MessageDeclaration(const MessageDeclaration&) = delete;
  MessageDeclaration &operator=(const MessageDeclaration&) = delete;
  MessageDeclaration(MessageDeclaration &&) = default;
  MessageDeclaration &operator=(MessageDeclaration &&) = default;

  Type t() const override;
  const std::string &name() const override;
  const DocCommentContext &doc() const override;
  const AST &ast() const override;
  const Token &start_token() const override;
 private:
  std::unique_ptr<Token> start_token_;
  std::string name_;
  std::unique_ptr<DocCommentContext> doc_;
  std::weak_ptr<AST> ast_;
};
}

#endif //TELL_COMPILER_MESSAGEDECLARATION_H_
