//
// Created by ANDREW SMITH on 25/04/2020.
//

#ifndef TELL_COMPILER_MAPTYPEDEF_H_
#define TELL_COMPILER_MAPTYPEDEF_H_

#include "TypeDef.h"
#include "SubTypeDef.h"

namespace til {
class MapTypeDef: public TypeDef, public SubTypeDef {
 public:
  MapTypeDef(std::unique_ptr<TypeDef> sub_type, bool optional);
  [[nodiscard]] bool optional() const override;
  [[nodiscard]] Type t() const override;
};
}

#endif //TELL_COMPILER_MAPTYPEDEF_H_
