//
// Created by ANDREW SMITH on 25/04/2020.
//

#ifndef TELL_COMPILER_MESSAGETYPEDEF_H_
#define TELL_COMPILER_MESSAGETYPEDEF_H_

#include "TypeDef.h"

namespace til {
class MessageTypeDef: public TypeDef {
 public:
  bool optional() const override;
  Type t() const override;

};
}

#endif //TELL_COMPILER_MESSAGETYPEDEF_H_
