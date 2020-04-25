#ifndef TELL_COMPILER_LISTTYPEDEF_H_
#define TELL_COMPILER_LISTTYPEDEF_H_

#include "TypeDef.h"

namespace til {
class ListTypeDef: public TypeDef {
 public:
  bool optional() const override;
  Type t() const override;
};
}

#endif //TELL_COMPILER_LISTTYPEDEF_H_
