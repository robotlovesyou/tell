#ifndef TELL_GEN_GOGENERATOR_H_
#define TELL_GEN_GOGENERATOR_H_

#include <string>
#include "GoTemplate.h"
#include "SerializableAST.h"

namespace gen {
class GoGenerator {
 public:
  std::string Generate(til::SerializableAST & ast);
 private:
  void EnsureDirective(til::SerializableAST &ast, const std::string& name);
};
}

#endif //TELL_GEN_GOGENERATOR_H_
