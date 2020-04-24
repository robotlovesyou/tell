//
// Created by ANDREW SMITH on 21/04/2020.
//

#ifndef TELL_COMPILER_AST_H_
#define TELL_COMPILER_AST_H_

#include <vector>
#include "Declaration.h"

namespace til {
class AST {
 public:
  int DeclarationCount() const;
  void AddDeclaration(std::unique_ptr<Declaration> decl);
  const Declaration *Declaration(int idx);
 private:
  std::vector<std::unique_ptr<til::Declaration>> declarations_;
};
}

#endif //TELL_COMPILER_AST_H_
