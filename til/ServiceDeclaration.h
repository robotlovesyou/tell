#ifndef TELL_COMPILER_SERVICEDECLARATION_H_
#define TELL_COMPILER_SERVICEDECLARATION_H_

#include <map>
#include <memory>
#include <vector>

#include "Declaration.h"
#include "Call.h"

namespace til {
struct SerializableServiceDeclaration; // forward declaration of SerializableServiceDeclaration
/**
 * Represents a service top level declaration in the AST
 */
class ServiceDeclaration: public Declaration {
 public:
  ServiceDeclaration(std::unique_ptr<Token> start_token, std::unique_ptr<DocCommentContext> doc, std::string name, std::vector<std::unique_ptr<Call>> calls, const std::shared_ptr<AST>& ast);

  // Disallow copy construction and assignment. Allow move.
  ~ServiceDeclaration() override = default;
  ServiceDeclaration(const ServiceDeclaration&) = delete;
  ServiceDeclaration &operator=(const ServiceDeclaration&) = delete;
  ServiceDeclaration(ServiceDeclaration &&) = default;
  ServiceDeclaration &operator=(ServiceDeclaration &&) = default;

  /**
   * Getters
   */
  [[nodiscard]] Type t() const override;
  [[nodiscard]] const Token &start_token() const override;
  [[nodiscard]] const std::string &name() const override;
  [[nodiscard]] const DocCommentContext &doc() const override;
  [[nodiscard]] const AST &ast() const override;
  [[nodiscard]] std::string t_name() const override;

  /**
   * Get the count of calls in this service
   * @return
   */
  [[nodiscard]] int CallCount() const;

  /**
   * Get the call at the provided index
   * @param idx
   * @return
   */
  [[nodiscard]] const Call *Call(int idx) const;

  /**
   * Convert the entire service structure to a SerializableServiceDeclaration
   * @return
   */
  [[nodiscard]] std::unique_ptr<SerializableServiceDeclaration> ToSerializable() const;
 private:
  /**
   * Add each call to an index by name and ensure that the call name is unique within the service
   */
  void AddCallsToIndex();

  std::unique_ptr<Token> start_token_;
  std::string name_;
  std::unique_ptr<DocCommentContext> doc_;
  std::weak_ptr<AST> ast_;
  std::vector<std::unique_ptr<til::Call>> calls_;
  std::map<std::string, int> call_index_;
};
}

#endif //TELL_COMPILER_SERVICEDECLARATION_H_
