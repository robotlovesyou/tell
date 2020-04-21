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
  MessageDeclaration(Token &&start_token, DocCommentContext &&doc, std::string name, const std::shared_ptr<AST> &ast);
  Type t() override;
  std::string name() override;
  DocCommentContext doc();
  std::shared_ptr<AST> ast() override;
 private:
  Token start_token_;
  std::string name_;
  DocCommentContext doc_;
  std::weak_ptr<AST> ast_;
};
}

#endif //TELL_COMPILER_MESSAGEDECLARATION_H_
