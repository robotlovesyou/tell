#ifndef TELL_COMPILER_ERRORREPORTER_H_
#define TELL_COMPILER_ERRORREPORTER_H_

#include <string>
#include "fmt/core.h"

namespace til {
/**
 * Abstract class implemented by all error reporters
 */
class ErrorReporter {
 public:
  /**
   * Report an error. Should set has_errors to true
   * @param message
   */

  virtual void ReportError(const std::string& message) = 0;

  /**
   * Should return true if the error reporter has had any errors reported
   * @return
   */
  virtual bool has_errors() = 0;
};
}

#endif //TELL_COMPILER_ERRORREPORTER_H_
