//
// Created by ANDREW SMITH on 14/04/2020.
//

#include "ErrorReporter.h"
#include "fmt/core.h"
#include <iostream>

void til::ErrorReporter::ReportError(const std::string& message) {
  has_errors_ = true;
  std::cerr << fmt::format("ERROR: {}", message) << std::endl;
}

void til::ErrorReporter::ReportWarning(const std::string& message) {
  std::cout << fmt::format("WARNING: {}", message) << std::endl;
}

bool til::ErrorReporter::has_errors() {
  return has_errors_;
}
