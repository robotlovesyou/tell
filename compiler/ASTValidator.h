//
// Created by ANDREW SMITH on 16/05/2020.
//

#ifndef TELL_COMPILER_ASTVALIDATOR_H_
#define TELL_COMPILER_ASTVALIDATOR_H_

#include <memory>

#include "AST.h"
#include "ErrorReporter.h"
#include "Field.h"
#include "ListTypeDef.h"
#include "MessageDeclaration.h"
#include "MessageTypeDef.h"
#include "MapTypeDef.h"
#include "ServiceDeclaration.h"


namespace til {
class ASTValidator {
 public:
  explicit ASTValidator(std::shared_ptr<ErrorReporter> error_reporter, AST * ast);
  void Validate();
 private:
  void ResolveMessages();
  void ResolveMessageFields(const MessageDeclaration *md);
  void ResolveTypeDef(const TypeDef *td, const Field * f, const MessageDeclaration *md);
  void ResolveMessageTypeDef(const MessageTypeDef *mtd, const Field * f, const MessageDeclaration *md);
  void ResolveListTypeDef(const ListTypeDef *ltd, const Field * f, const MessageDeclaration *md);
  void ResolveMapTypeDef(const MapTypeDef *mtd, const Field * f, const MessageDeclaration *md);
  void ResolveAllServiceArgs();
  void ResolveServiceArgs(const ServiceDeclaration *sd);
  void ResolveCallArguments(const Call *call, const ServiceDeclaration *sd);
  std::shared_ptr<ErrorReporter> error_reporter_;
  AST *ast_;
};
}

#endif //TELL_COMPILER_ASTVALIDATOR_H_
