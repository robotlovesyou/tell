//
// Created by ANDREW SMITH on 21/04/2020.
//

#ifndef TELL_COMPILER_DIRECTIVEDECLARATION_H_
#define TELL_COMPILER_DIRECTIVEDECLARATION_H_

#include <memory>
#include "Declaration.h"
#include "Token.h"
#include "DocCommentContext.h"

namespace til {
class DirectiveDeclaration : public Declaration {
 public:
  DirectiveDeclaration(std::unique_ptr<Token> start_token,
                       std::unique_ptr<til::DocCommentContext> doc,
                       std::string name,
                       std::string value,
                       const std::shared_ptr<AST>& ast);


  // Disallow copy construction and assignment. Allow move.
  ~DirectiveDeclaration() = default;
  DirectiveDeclaration(const DirectiveDeclaration&) = delete;
  DirectiveDeclaration &operator=(const DirectiveDeclaration&) = delete;
  DirectiveDeclaration(DirectiveDeclaration &&) = default;
  DirectiveDeclaration &operator=(DirectiveDeclaration &&) = default;

  [[nodiscard]] Declaration::Type t() const override;
  [[nodiscard]] const std::string &name() const override;
  [[nodiscard]] const std::string &value() const;
  [[nodiscard]] const AST &ast() const override;
  [[nodiscard]] const DocCommentContext &doc() const override;
  [[nodiscard]] const Token &start_token() const override;
  [[nodiscard]] std::string t_name() const override;
 private:
  std::unique_ptr<Token> start_token_;
  std::unique_ptr<DocCommentContext> doc_;
  std::string name_;
  std::string value_;
  std::weak_ptr<AST> ast_;
};
}

#endif //TELL_COMPILER_DIRECTIVEDECLARATION_H_
