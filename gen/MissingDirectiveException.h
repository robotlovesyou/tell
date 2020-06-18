#ifndef TELL_GEN_MISSINGDIRECTIVEEXCEPTION_H_
#define TELL_GEN_MISSINGDIRECTIVEEXCEPTION_H_

#include <exception>
#include <string>

namespace gen {
class MissingDirectiveException: public std::exception {
 public:
  explicit MissingDirectiveException(const std::string& message);
  [[nodiscard]] const char * what() const noexcept override;
 private:
  std::string message_;
};
}

#endif //TELL_GEN_MISSINGDIRECTIVEEXCEPTION_H_
