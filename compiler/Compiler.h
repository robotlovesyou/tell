#ifndef TELL_COMPILER_COMPILER_H_
#define TELL_COMPILER_COMPILER_H_

#include <exception>
#include <memory>
#include <string>

#include "ErrorReporter.h"

class Compiler {
 public:
  Compiler(std::string f, std::string o, std::shared_ptr<til::ErrorReporter>  error_reporter, bool pretty);

 class OutputException: public std::exception {
  public:
   explicit OutputException(std::string reason);
   [[nodiscard]] const char * what() const noexcept override;
  private:
   std::string reason_;
 };
  bool Compile();
 private:
  std::ofstream OpenOutFile();

  std::shared_ptr<til::ErrorReporter> error_reporter_;
  std::string file_;
  std::string out_;
  bool pretty_;
};

#endif //TELL_COMPILER_COMPILER_H_
