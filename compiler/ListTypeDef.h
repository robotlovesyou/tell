#ifndef TELL_COMPILER_LISTTYPEDEF_H_
#define TELL_COMPILER_LISTTYPEDEF_H_

#include "TypeDef.h"
#include "SubTypeDef.h"

namespace til {
class ListTypeDef: public TypeDef, public SubTypeDef {
 public:
  ListTypeDef(std::unique_ptr<TypeDef> sub_type, bool optional);
  bool optional() const override;
  Type t() const override;
};
}

#endif //TELL_COMPILER_LISTTYPEDEF_H_
