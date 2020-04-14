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
  void ReportError(const std::string& message);
  void ReportWarning(const std::string& message);
  bool has_errors();
 private:
  bool has_errors_{};
};
}

#endif //TELL_COMPILER_ERRORREPORTER_H_
