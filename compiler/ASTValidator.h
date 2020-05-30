//
// Created by ANDREW SMITH on 16/05/2020.
//

#ifndef TELL_COMPILER_ASTVALIDATOR_H_
#define TELL_COMPILER_ASTVALIDATOR_H_

#include <exception>
#include <memory>
#include <set>

#include "AST.h"
#include "ErrorReporter.h"
#include "Field.h"
#include "ListTypeDef.h"
#include "MessageDeclaration.h"
#include "MessageTypeDef.h"
#include "MapTypeDef.h"
#include "ServiceDeclaration.h"

namespace til {
/**
 * ASTValidator runs checks to ensure the parsed AST is valid.
 * It ensures all referenced messages in fields and service calls are declared
 * and it ensures that no message is part of an infinitely recursive loop
 */
class ASTValidator {
 public:
  explicit ASTValidator(std::shared_ptr<ErrorReporter> error_reporter, AST *ast);
  /**
   * Validate reports errors to the error reporter
   */
  void Validate();
 private:

  /**
   * InfiniteRecursionError is thrown when an infinitely recursive message structure is found
   */
  class InfiniteRecursionError : public std::exception {
   public:
    explicit InfiniteRecursionError(std::vector<std::string> members);
    const char *what();
   private:
    std::string message_;
  };

  /**
   * Ensure each message in a field or service call is declared as a message type
   */
  void ResolveMessages();

  /**
   * Iterate over each declared message looking for infinite recursions
   */
  void FindInfiniteRecursions() const;

  /**
   * Perform a depth first search, starting at the provided message
   * and looking for any infinite recursions
   * @param md The starting point of the search
   * @param visited a set of nodes which have already been checked and can be skipped
   * @param stack the current stack of nodes in the search
   */
  void FindInfiniteRecursionsInMessage(const MessageDeclaration *md,
                                       std::set<int> *visited,
                                       std::vector<std::string> *stack) const;
  std::vector<std::string>CollectRecursionMembers(std::vector<std::string>::iterator it, std::vector<std::string>*stack, const std::string& tail) const;

  /**
   * Resolve the message referenced in each field of the provided message
   * @param md
   */
  void ResolveMessageFields(const MessageDeclaration *md);

  /**
   * Resolve any message referenced in the provided type definition
   * @param td The type definition to test
   * @param f The owning field of td
   * @param md The owning message of f
   */
  void ResolveTypeDef(const TypeDef *td, const Field *f, const MessageDeclaration *md);

  /**
   * Check that the message referred to in this message type defintion exists
   * @param mtd The message type declaration to check
   * @param f The owning field of mtd
   * @param md The owning message of f
   */
  void ResolveMessageTypeDef(const MessageTypeDef *mtd, const Field *f, const MessageDeclaration *md);

  /**
   * Recursively check that any message referred to as a sub type of this list exists
   * @param ltd The list type declaration to check
   * @param f The owning field of ltd
   * @param md The owning message of f
   */
  void ResolveListTypeDef(const ListTypeDef *ltd, const Field *f, const MessageDeclaration *md);

  /**
   * Recursively check that any message referred to as a sub type of this map exists
   * @param mtd The map type declaration to check
   * @param f The owning field of mtd
   * @param md The owning message of f
   */
  void ResolveMapTypeDef(const MapTypeDef *mtd, const Field *f, const MessageDeclaration *md);

  /**
   * Check that all messages referred to by all services exist
   */
  void ResolveAllServiceArgs();

  /**
   * Check that all messages referred to by this service exist
   * @param sd The service to check
   */
  void ResolveServiceArgs(const ServiceDeclaration *sd);

  /**
   * Check that all messages referred to by this call exist
   * @param call The call to check
   * @param sd The owning service of call
   */
  void ResolveCallArguments(const Call *call, const ServiceDeclaration *sd);

  std::shared_ptr<ErrorReporter> error_reporter_;
  AST *ast_;
};
}

#endif //TELL_COMPILER_ASTVALIDATOR_H_
