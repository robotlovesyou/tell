//
// Created by ANDREW SMITH on 25/04/2020.
//

#ifndef TELL_COMPILER_MAPTYPEDEF_H_
#define TELL_COMPILER_MAPTYPEDEF_H_

#include "TypeDef.h"
#include "SubTypeDef.h"

namespace til {
/**
 * TypeDef implementation to represent map types in the AST
 */
class MapTypeDef: public TypeDef, public SubTypeDef {
 public:
  MapTypeDef(std::unique_ptr<TypeDef> sub_type, bool optional);

  /**
   * Getters
   * @return
   */
  [[nodiscard]] bool optional() const override;
  [[nodiscard]] Type t() const override;

  /**
   * Convert the map type def to a SerializableTypeDef
   * @return
   */
  [[nodiscard]] std::unique_ptr<SerializableTypeDef> ToSerializable() const override;
};
}

#endif //TELL_COMPILER_MAPTYPEDEF_H_
