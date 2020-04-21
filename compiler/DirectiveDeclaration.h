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
  DirectiveDeclaration(Token &&startToken,
                       til::DocCommentContext &&doc,
                       std::string name,
                       std::string value,
                       std::shared_ptr<AST> ast);

  Declaration::Type t() override;
  std::string name() override;
  std::shared_ptr<AST> ast() override;
};
}

#endif //TELL_COMPILER_DIRECTIVEDECLARATION_H_
