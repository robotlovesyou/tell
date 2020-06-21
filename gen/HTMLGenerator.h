#ifndef TELL_GEN_HTMLGENERATOR_H_
#define TELL_GEN_HTMLGENERATOR_H_

#include <string>

#include "inja/inja.hpp"

#include "Generator.h"
#include "SerializableAST.h"

namespace gen {
/**
 * Generates HTML Documentation from the provided AST
 */
class HTMLGenerator: public Generator {
 public:
  /**
   * Generate HTML Documentation and return it as a string
   * @param ast
   * @return
   */
  std::string Generate(const til::SerializableAST &ast) override;

 private:
  /**
   * Set up template callbacks and rendering options
   * @return
   */
  inja::Environment PrepareEnvironment();

  /**
   * Return a string representation for the provided type def.
   * @param td
   * @return
   */
  std::string MapType(const til::SerializableTypeDef *td);
  std::string MapType(const til::ScalarSerializableTypeDef *td);
  std::string MapType(const til::MessageSerializableTypeDef *td);
  std::string MapType(const til::MapSerializableTypeDef *td);
  std::string MapType(const til::ListSerializableTypeDef *td);
};
}

#endif //TELL_GEN_HTMLGENERATOR_H_
