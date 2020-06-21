#ifndef TELL_COMPILER_LISTTYPEDEF_H_
#define TELL_COMPILER_LISTTYPEDEF_H_

#include "TypeDef.h"
#include "SubTypeDef.h"

namespace til {
/**
 * TypeDef implementation to represent List types in the AST
 */
class ListTypeDef: public TypeDef, public SubTypeDef {
 public:
  ListTypeDef(std::unique_ptr<TypeDef> sub_type, bool optional);

  /**
   * Getters
   */
  [[nodiscard]] bool optional() const override;
  [[nodiscard]] Type t() const override;

  /**
   * Convert the type def structure to a SerializableTypeDef
   * @return
   */
  [[nodiscard]] std::unique_ptr<SerializableTypeDef> ToSerializable() const override;
};
}

#endif //TELL_COMPILER_LISTTYPEDEF_H_
