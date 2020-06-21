#include "ConsoleErrorReporter.h"
#include <iostream>

void til::ConsoleErrorReporter::ReportError(const std::string& message) {
  has_errors_ = true;
  std::cerr << fmt::format("ERROR: {}", message) << std::endl;
}

bool til::ConsoleErrorReporter::has_errors() {
  return has_errors_;
}
