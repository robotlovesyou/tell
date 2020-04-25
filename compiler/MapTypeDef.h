//
// Created by ANDREW SMITH on 25/04/2020.
//

#ifndef TELL_COMPILER_MAPTYPEDEF_H_
#define TELL_COMPILER_MAPTYPEDEF_H_

#include "TypeDef.h"

namespace til {
class MapTypeDef: TypeDef {
  bool optional() const override;
  Type t() const override;
};
}

#endif //TELL_COMPILER_MAPTYPEDEF_H_
