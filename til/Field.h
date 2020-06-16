//
// Created by ANDREW SMITH on 25/04/2020.
//

#ifndef TELL_COMPILER_FIELD_H_
#define TELL_COMPILER_FIELD_H_

#include <memory>
#include <string>

#include "DocCommentContext.h"
#include "TypeDef.h"

namespace til {
struct SerializableField; // forward declaration of serializable field
class Field {
 public:
  Field(std::string name, std::unique_ptr<TypeDef> type_def, std::unique_ptr<DocCommentContext> doc);
  [[nodiscard]] std::string name() const;
  [[nodiscard]] const TypeDef *type_def() const;
  [[nodiscard]] const DocCommentContext &doc() const;
  [[nodiscard]] std::unique_ptr<SerializableField> ToSerializable() const;
 private:
  std::string name_;
  std::unique_ptr<TypeDef> type_def_;
  std::unique_ptr<DocCommentContext> doc_;
};
}

#endif //TELL_COMPILER_FIELD_H_
