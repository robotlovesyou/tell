#ifndef TELL_GEN_GOGENERATOR_H_
#define TELL_GEN_GOGENERATOR_H_

#include <string>

#include "inja/inja.hpp"
#include "GoTemplate.h"
#include "SerializableAST.h"

namespace gen {
class GoGenerator {
 public:
  std::string Generate(til::SerializableAST & ast);
 private:
  void EnsureDirective(til::SerializableAST &ast, const std::string& name);
  inja::Environment PrepareEnvironment(til::SerializableAST &ast);
  std::string GoPackageName(inja::Arguments &args);
  std::string MapType(const til::SerializableTypeDef *td);
  std::string MapType(const til::ScalarSerializableTypeDef *td);
  std::string MapType(const til::MessageSerializableTypeDef *td);
  std::string MapType(const til::MapSerializableTypeDef *td);
  std::string MapType(const til::ListSerializableTypeDef *td);
};
}

#endif //TELL_GEN_GOGENERATOR_H_
