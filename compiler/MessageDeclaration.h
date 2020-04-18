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
  MessageDeclaration(Token &&start_token, DocCommentContext &&doc, std::string name);
  Type t() override;
  std::string name() override;
  DocCommentContext doc();
 private:
  Token start_token_;
  std::string name_;
  std::optional<DocCommentContext> doc_;
};
}

#endif //TELL_COMPILER_MESSAGEDECLARATION_H_
