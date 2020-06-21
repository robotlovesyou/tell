#ifndef TELL_COMPILER_COMPILER_H_
#define TELL_COMPILER_COMPILER_H_

#include <exception>
#include <memory>
#include <string>

#include "ErrorReporter.h"

/**
 * Compiler loads til source code and compiles it to an ast if valid
 */
class Compiler {
 public:
  /**
   * @param f the source file
   * @param o the output file for the ast JSON
   * @param error_reporter
   * @param pretty pretty prints the json to the output file if true
   */
  Compiler(std::string f, std::string o, std::shared_ptr<til::ErrorReporter> error_reporter, bool pretty);

  /**
   * Thrown if the compiler cannot write to the output file
   */
  class OutputException : public std::exception {
   public:
    explicit OutputException(std::string reason);
    [[nodiscard]] const char *what() const noexcept override;
   private:
    std::string reason_;
  };

  /**
   * Compile the source to an AST
   * @return
   */
  bool Compile();
 private:
  /**
   * Open the output file and return the linked ofstream
   * @return
   */
  std::ofstream OpenOutFile();

  std::shared_ptr<til::ErrorReporter> error_reporter_;
  std::string file_;
  std::string out_;
  bool pretty_;
};

#endif //TELL_COMPILER_COMPILER_H_
