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

  // Set rule of 5 methods and default constructor to default.
  // See https://stackoverflow.com/questions/26039907/does-rule-of-three-five-apply-to-inheritance-and-virtual-destructors
  virtual ~Declaration() = default;
  Declaration() = default;
  Declaration(const Declaration&) = default;
  Declaration &operator=(const Declaration&) = default;
  Declaration(Declaration &&) = default;
  Declaration &operator=(Declaration &&) = default;

  virtual Type t() const = 0;
  virtual const Token &start_token() const = 0;
  virtual const std::string &name() const = 0;
  virtual const DocCommentContext &doc() const = 0;
  virtual const AST &ast() const = 0;
};
}


#endif //TELL_COMPILER_DECLARATION_H_
