#ifndef TELL_COMPILER_DECLARATION_H_
#define TELL_COMPILER_DECLARATION_H_

#include <memory>
#include <string>
#include "DocCommentContext.h"
#include "Token.h"

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
  virtual const Token &start_token() = 0;
  virtual const std::string &name() = 0;
  virtual const DocCommentContext &doc() = 0;
  virtual const AST &ast() = 0;
};
}


#endif //TELL_COMPILER_DECLARATION_H_
