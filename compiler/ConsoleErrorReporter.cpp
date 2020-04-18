//
// Created by ANDREW SMITH on 18/04/2020.
//

#include "ConsoleErrorReporter.h"
#include <iostream>

void til::ConsoleErrorReporter::ReportError(const std::string& message) {
  has_errors_ = true;
  std::cerr << fmt::format("ERROR: {}", message) << std::endl;
}

void til::ConsoleErrorReporter::ReportWarning(const std::string& message) {
  std::cout << fmt::format("WARNING: {}", message) << std::endl;
}

bool til::ConsoleErrorReporter::has_errors() {
  return has_errors_;
}
