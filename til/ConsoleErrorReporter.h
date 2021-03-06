#ifndef TELL_COMPILER_CONSOLEERRORREPORTER_H_
#define TELL_COMPILER_CONSOLEERRORREPORTER_H_

#include "ErrorReporter.h"

namespace til {
/**
 * Error reporter which reports errors to the console
 */
 class ConsoleErrorReporter: public til::ErrorReporter {
  public:
   void ReportError(const std::string& message) override;
   bool has_errors() override;
  private:
   bool has_errors_{false};
};
}

#endif //TELL_COMPILER_CONSOLEERRORREPORTER_H_
