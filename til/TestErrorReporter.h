//
// Created by ANDREW SMITH on 18/04/2020.
//

#ifndef TELL_COMPILER_TESTERRORREPORTER_H_
#define TELL_COMPILER_TESTERRORREPORTER_H_

#include "ErrorReporter.h"

namespace til {
/**
 * Error reporter implementation used in tests.
 */
class TestErrorReporter : public til::ErrorReporter {
 public:
  void ReportError(const std::string &message) override;
  bool has_errors() override;
 private:
  bool has_errors_{false};

};
}

#endif //TELL_COMPILER_TESTERRORREPORTER_H_
