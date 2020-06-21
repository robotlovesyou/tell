#ifndef TELL_COMPILER_TYPEDEF_H_
#define TELL_COMPILER_TYPEDEF_H_

#include <memory>

namespace til {
// forward declaration of serializable type def
struct SerializableTypeDef;

/**
 * Abstract class implemented by all type defs (scalar, message, list and map)
 */
class TypeDef {
 public:
  /**
   * The range of type defs
   */
  enum Type {
    kScalar,
    kMessage,
    kMap,
    kList,
  };

  [[nodiscard]] virtual bool optional() const;
  [[nodiscard]] virtual Type t() const = 0;
  [[nodiscard]] virtual std::unique_ptr<SerializableTypeDef> ToSerializable() const = 0;

  // Set rule of 5 methods and default constructor to default.
  // See https://stackoverflow.com/questions/26039907/does-rule-of-three-five-apply-to-inheritance-and-virtual-destructors
  virtual ~TypeDef() = default;
  TypeDef(const TypeDef&) = delete;
  TypeDef &operator=(const TypeDef&) = delete;
  TypeDef (TypeDef &&) = default;
  TypeDef &operator=(TypeDef &&) = default;

 protected:
  explicit TypeDef(bool optional);
  bool optional_;
};
}

#endif //TELL_COMPILER_TYPEDEF_H_
