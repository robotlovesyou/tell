#ifndef TELL_GEN_GOGENERATOR_H_
#define TELL_GEN_GOGENERATOR_H_

#include <string>

#include "inja/inja.hpp"
#include "Generator.h"
#include "GoTemplate.h"
#include "SerializableAST.h"

/**
 * At present there are no tests for this generator code.
 *
 * I started writing tests but they just end up as huge string comparisons and very brittle.
 *
 * In a real world implementation a good approach might be to generate go code and then
 * compile and test it using the Go toolchain.
 */

namespace gen {
 class GoGenerator: public Generator {
 public:
  /**
   * Transform the provided AST into a Golang representation
   * of the described RPCs
   * @param ast
   * @return
   */
  std::string Generate(const til::SerializableAST & ast) override;
 private:

  /**
   * Ensure the named directive exists. Raise an error if not
   * @param ast
   * @param name
   */
  void EnsureDirective(const til::SerializableAST &ast, const std::string& name);

  /**
   * Prepare the inja template environment by adding required callback
   * functions and setting required options
   * @param ast
   * @return
   */
  inja::Environment PrepareEnvironment(const til::SerializableAST &ast);

  /**
   * Transform the provided argument into a valid go package name
   * @param args
   * @return
   */
  std::string GoPackageName(inja::Arguments &args);

  /**
   * Return the go encoding for the provided type def.
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

#endif //TELL_GEN_GOGENERATOR_H_
