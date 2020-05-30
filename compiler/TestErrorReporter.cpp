//
// Created by ANDREW SMITH on 18/04/2020.
//

#include "TestErrorReporter.h"
#include <iostream>

void til::TestErrorReporter::ReportError(const std::string &message) {
  has_errors_ = true;
  std::cout << "The following message is test output. It is only an error if tests also fail" << std::endl;
  std::cout << fmt::format("ERROR: {}", message) << std::endl;
}

bool til::TestErrorReporter::has_errors() {
  return has_errors_;
}
