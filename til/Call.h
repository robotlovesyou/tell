#ifndef TELL_COMPILER_CALL_H_
#define TELL_COMPILER_CALL_H_

#include <memory>
#include <string>
#include "Argument.h"
#include "DocCommentContext.h"

namespace til {
struct SerializableCall; // forward declaration of SerializableCall

/**
 * Represents a service call in the AST
 */
class Call {
 public:
  Call(std::string name, std::unique_ptr<DocCommentContext> doc, std::unique_ptr<Argument>argument, std::unique_ptr<Argument>returns);

  /**
   * Getters
   */
  [[nodiscard]] std::string name() const;
  [[nodiscard]] const DocCommentContext &doc() const;
  [[nodiscard]] const Argument &argument() const;
  [[nodiscard]] const Argument &returns() const;

  /**
   * Convert the call structure to a SerializableCall
   * @return
   */
  [[nodiscard]] std::unique_ptr<SerializableCall> ToSerializable() const;
 private:
  std::string name_;
  std::unique_ptr<Argument> argument_;
  std::unique_ptr<Argument> returns_;
  std::unique_ptr<DocCommentContext> doc_;
};
}

#endif //TELL_COMPILER_CALL_H_
