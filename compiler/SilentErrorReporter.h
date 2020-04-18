//
// Created by ANDREW SMITH on 18/04/2020.
//

#ifndef TELL_COMPILER_SILENTERRORREPORTER_H_
#define TELL_COMPILER_SILENTERRORREPORTER_H_

#include "ErrorReporter.h"

namespace til {
class SilentErrorReporter : public til::ErrorReporter {
 public:
  void ReportError(const std::string &message) override;
  void ReportWarning(const std::string &message) override {} //;
  bool has_errors() override;
 private:
  bool has_errors_{false};

};
}

#endif //TELL_COMPILER_SILENTERRORREPORTER_H_
