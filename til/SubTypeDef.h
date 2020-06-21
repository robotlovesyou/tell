#ifndef TELL_COMPILER_SUBTYPEDEF_H_
#define TELL_COMPILER_SUBTYPEDEF_H_

#include "TypeDef.h"

#include <memory>

namespace til {
/**
 * Abstract class  implemented by all type defs which contain other types (map and list)
 */
class SubTypeDef {
 public:

  explicit SubTypeDef(std::unique_ptr<TypeDef> sub_type);
  [[nodiscard]] const TypeDef *sub_type() const;
 private:
  std::unique_ptr<TypeDef> sub_type_;
};
}

#endif //TELL_COMPILER_SUBTYPEDEF_H_
