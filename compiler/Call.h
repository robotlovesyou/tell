//
// Created by ANDREW SMITH on 09/05/2020.
//

#ifndef TELL_COMPILER_CALL_H_
#define TELL_COMPILER_CALL_H_

#include <memory>
#include <string>
#include "Argument.h"
#include "DocCommentContext.h"

namespace til {
class Call {
 public:
  Call(std::string name, std::unique_ptr<DocCommentContext> doc, std::unique_ptr<Argument>argument, std::unique_ptr<Argument>returns);
  [[nodiscard]] std::string name() const;
  [[nodiscard]] const DocCommentContext &doc() const;
  [[nodiscard]] const Argument &argument() const;
  [[nodiscard]] const Argument &returns() const;
 private:
  std::string name_;
  std::unique_ptr<Argument> argument_;
  std::unique_ptr<Argument> returns_;
  std::unique_ptr<DocCommentContext> doc_;
};
}

#endif //TELL_COMPILER_CALL_H_
