//
// Created by ANDREW SMITH on 25/04/2020.
//

#ifndef TELL_COMPILER_MESSAGETYPEDEF_H_
#define TELL_COMPILER_MESSAGETYPEDEF_H_

#include "TypeDef.h"

#include <string>

namespace til {
class AST; // forward declaration of AST
class MessageTypeDef: public TypeDef {
 public:
  // TODO: AST is required for later validation of the message
  MessageTypeDef(std::string name, const std::shared_ptr<AST>& ast, bool optional);
  [[nodiscard]] bool optional() const override;
  [[nodiscard]] Type t() const override;
  [[nodiscard]] const std::string &name() const;
 private:
  std::string name_;
  std::weak_ptr<AST> ast_;
};
}

#endif //TELL_COMPILER_MESSAGETYPEDEF_H_
