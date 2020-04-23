//
// Created by ANDREW SMITH on 23/04/2020.
//

#ifndef TELL_COMPILER_SERVICEDECLARATION_H_
#define TELL_COMPILER_SERVICEDECLARATION_H_

#import "Declaration.h"

namespace til {
class ServiceDeclaration: public Declaration {
 public:
  ServiceDeclaration(std::unique_ptr<Token> start_token, std::unique_ptr<DocCommentContext> doc, std::string name, const std::shared_ptr<AST>& ast);
  Type t() override;
  const Token &start_token() override;
  const std::string &name() override;
  const DocCommentContext &doc() override;
  const AST &ast() override;
 private:
  std::unique_ptr<Token> start_token_;
  std::string name_;
  std::unique_ptr<DocCommentContext> doc_;
  std::weak_ptr<AST> ast_;
};
}

#endif //TELL_COMPILER_SERVICEDECLARATION_H_
