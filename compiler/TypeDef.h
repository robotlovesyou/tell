#ifndef TELL_COMPILER_TYPEDEF_H_
#define TELL_COMPILER_TYPEDEF_H_

namespace til {
class TypeDef {
 public:
  enum Type {
    kScalar,
    kMessage,
    kMap,
    kList,
  };

  [[nodiscard]] virtual bool optional() const;
  [[nodiscard]] virtual Type t() const = 0;

  // Set rule of 5 methods and default constructor to default.
  // See https://stackoverflow.com/questions/26039907/does-rule-of-three-five-apply-to-inheritance-and-virtual-destructors
  virtual ~TypeDef() = default;
  TypeDef(const TypeDef&) = delete;
  TypeDef &operator=(const TypeDef&) = delete;
  TypeDef (TypeDef &&) = default;
  TypeDef &operator=(TypeDef &&) = default;

 protected:
  TypeDef(bool optional);
  bool optional_;
};
}

#endif //TELL_COMPILER_TYPEDEF_H_
