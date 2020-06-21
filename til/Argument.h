#ifndef TELL_COMPILER_ARGUMENT_H_
#define TELL_COMPILER_ARGUMENT_H_

#include <memory>
#include <string>

namespace til {
class AST; // forward declaration of ast

/**
 * Represents a call argument or return type in the AST
 */
class Argument {
 public:
  Argument(std::string name, const std::shared_ptr<AST> &ast);

  /**
   * The name of the argument
   * @return
   */
  [[nodiscard]] std::string name() const;

  // disallow copy and explicitly set move and destructor to default
  virtual ~Argument() = default;
  Argument(const Argument&) = delete;
  Argument &operator=(const Argument&) = delete;
  Argument (Argument &&) = default;
  Argument &operator=(Argument &&) = default;
 private:
  std::string name_;
  std::weak_ptr<AST> ast_;
};
}

#endif //TELL_COMPILER_ARGUMENT_H_
