#ifndef TELL_COMPILER_SUBTYPEDEF_H_
#define TELL_COMPILER_SUBTYPEDEF_H_

#include "TypeDef.h"

#include <memory>

namespace til {
class SubTypeDef {
 public:

  explicit SubTypeDef(std::unique_ptr<TypeDef> sub_type);
  const TypeDef *sub_type() const;
 private:
  std::unique_ptr<TypeDef> sub_type_;
};
}

#endif //TELL_COMPILER_SUBTYPEDEF_H_
