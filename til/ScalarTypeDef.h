#ifndef TELL_COMPILER_SCALARTYPEDEF_H_
#define TELL_COMPILER_SCALARTYPEDEF_H_

#include "TypeDef.h"

namespace til {
/**
 * TypeDef implementation for representing scalar types
 */
class ScalarTypeDef: public TypeDef {
 public:

  /**
   * The range of scalar types
   */
  enum ScalarType {
    kBool,
    kFloat,
    kInt,
    kString,
    kTime,
  };

  ScalarTypeDef(ScalarType st, bool optional);
  /**
   * Getters
   */
  [[nodiscard]] Type t() const override;
  [[nodiscard]] ScalarType scalar_type() const;

  /**
   * Convert the scalar type def to a SerializableTypeDef
   * @return
   */
  [[nodiscard]] std::unique_ptr<SerializableTypeDef> ToSerializable() const override;
 private:
  ScalarType scalar_type_;
};
}

#endif //TELL_COMPILER_SCALARTYPEDEF_H_
