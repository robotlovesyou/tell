#ifndef TELL_COMPILER_SERVICEDECLARATION_H_
#define TELL_COMPILER_SERVICEDECLARATION_H_

#import "Declaration.h"

namespace til {
class ServiceDeclaration: public Declaration {
 public:
  ServiceDeclaration(std::unique_ptr<Token> start_token, std::unique_ptr<DocCommentContext> doc, std::string name, const std::shared_ptr<AST>& ast);

  // Disallow copy construction and assignment. Allow move.
  ~ServiceDeclaration() = default;
  ServiceDeclaration(const ServiceDeclaration&) = delete;
  ServiceDeclaration &operator=(const ServiceDeclaration&) = delete;
  ServiceDeclaration(ServiceDeclaration &&) = default;
  ServiceDeclaration &operator=(ServiceDeclaration &&) = default;

  Type t() const override;
  const Token &start_token() const override;
  const std::string &name() const override;
  const DocCommentContext &doc() const override;
  const AST &ast() const override;
 private:
  std::unique_ptr<Token> start_token_;
  std::string name_;
  std::unique_ptr<DocCommentContext> doc_;
  std::weak_ptr<AST> ast_;
};
}

#endif //TELL_COMPILER_SERVICEDECLARATION_H_
