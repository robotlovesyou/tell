//
// Created by ANDREW SMITH on 25/04/2020.
//

#ifndef TELL_COMPILER_SCALARTYPEDEF_H_
#define TELL_COMPILER_SCALARTYPEDEF_H_

#include "TypeDef.h"

namespace til {
class ScalarTypeDef: public TypeDef {
 public:
  enum ScalarType {
    kBool,
    kFloat,
    kInt,
    kString,
    kTime,
  };

  ScalarTypeDef(ScalarType st, bool optional);
  [[nodiscard]] Type t() const override;
  ScalarType scalar_type() const;
 private:
  ScalarType scalar_type_;
};
}

#endif //TELL_COMPILER_SCALARTYPEDEF_H_
