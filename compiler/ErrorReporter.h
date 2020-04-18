//
// Created by ANDREW SMITH on 14/04/2020.
//

#ifndef TELL_COMPILER_ERRORREPORTER_H_
#define TELL_COMPILER_ERRORREPORTER_H_

#include <string>
#include "fmt/core.h"

namespace til {
class ErrorReporter {
 public:
  virtual void ReportError(const std::string& message) = 0;
  virtual void ReportWarning(const std::string& message) = 0;
  virtual bool has_errors() = 0;
};
}

#endif //TELL_COMPILER_ERRORREPORTER_H_
