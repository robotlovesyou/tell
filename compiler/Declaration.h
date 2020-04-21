#ifndef TELL_COMPILER_DECLARATION_H_
#define TELL_COMPILER_DECLARATION_H_

#include <memory>
#include <string>

namespace til {

/**
 * Forward declaration of AST class
 */
class AST;

/**
 * Declaration is the interface for AST classes representing top level declarations.
 */
class Declaration {
 public:
  /**
   * Declaration::Type defines the available types for AST top level declarations
   */
  enum Type {
    kDirective, kMessage, kService
  };

  virtual Type t() = 0;
  virtual std::string name() = 0;
  virtual std::shared_ptr<AST>ast() = 0;
};
}


#endif //TELL_COMPILER_DECLARATION_H_
