//
// Created by ANDREW SMITH on 25/04/2020.
//

#ifndef TELL_COMPILER_MESSAGETYPEDEF_H_
#define TELL_COMPILER_MESSAGETYPEDEF_H_

#include "TypeDef.h"

#include <string>

namespace til {
class AST; // forward declaration of AST

/**
 * TypeDef implementation to represent Message types
 */
class MessageTypeDef: public TypeDef {
 public:
  MessageTypeDef(std::string name, const std::shared_ptr<AST>& ast, bool optional);

  /**
   * Getters
   */
  [[nodiscard]] bool optional() const override;
  [[nodiscard]] Type t() const override;
  [[nodiscard]] const std::string &name() const;

  /**
   * Convert the message type def to a SerializableTypeDef
   * @return
   */
  [[nodiscard]] std::unique_ptr<SerializableTypeDef> ToSerializable() const override;
 private:
  std::string name_;
  std::weak_ptr<AST> ast_;
};
}

#endif //TELL_COMPILER_MESSAGETYPEDEF_H_
