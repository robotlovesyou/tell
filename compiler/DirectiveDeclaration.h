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

  Declaration::Type t() const override;
  const std::string &name() const override;
  const std::string &value() const;
  const AST &ast() const override;
  const DocCommentContext &doc() const override;
  const Token &start_token() const override;
 private:
  std::unique_ptr<Token> start_token_;
  std::unique_ptr<DocCommentContext> doc_;
  std::string name_;
  std::string value_;
  std::weak_ptr<AST> ast_;
};
}

#endif //TELL_COMPILER_DIRECTIVEDECLARATION_H_
