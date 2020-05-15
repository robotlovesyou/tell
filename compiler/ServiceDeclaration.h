#ifndef TELL_COMPILER_SERVICEDECLARATION_H_
#define TELL_COMPILER_SERVICEDECLARATION_H_

#import <map>
#import <memory>
#import <vector>

#import "Declaration.h"
#import "Call.h"

namespace til {
class ServiceDeclaration: public Declaration {
 public:
  ServiceDeclaration(std::unique_ptr<Token> start_token, std::unique_ptr<DocCommentContext> doc, std::string name, std::vector<std::unique_ptr<Call>> calls, const std::shared_ptr<AST>& ast);

  // Disallow copy construction and assignment. Allow move.
  ~ServiceDeclaration() override = default;
  ServiceDeclaration(const ServiceDeclaration&) = delete;
  ServiceDeclaration &operator=(const ServiceDeclaration&) = delete;
  ServiceDeclaration(ServiceDeclaration &&) = default;
  ServiceDeclaration &operator=(ServiceDeclaration &&) = default;

  [[nodiscard]] Type t() const override;
  [[nodiscard]] const Token &start_token() const override;
  [[nodiscard]] const std::string &name() const override;
  [[nodiscard]] const DocCommentContext &doc() const override;
  [[nodiscard]] const AST &ast() const override;
  [[nodiscard]] int CallCount() const;
  [[nodiscard]] const Call &Call(int idx) const;
  [[nodiscard]] std::string t_name() const override;
 private:
  std::unique_ptr<Token> start_token_;
  std::string name_;
  std::unique_ptr<DocCommentContext> doc_;
  std::weak_ptr<AST> ast_;
  std::vector<std::unique_ptr<til::Call>> calls_;
  std::map<std::string, int> call_index_;

  void AddCallsToIndex();
};
}

#endif //TELL_COMPILER_SERVICEDECLARATION_H_
