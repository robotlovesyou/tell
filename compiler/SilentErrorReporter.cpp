//
// Created by ANDREW SMITH on 18/04/2020.
//

#include "SilentErrorReporter.h"
void til::SilentErrorReporter::ReportError(const std::string &message) {
  has_errors_ = true;
}

bool til::SilentErrorReporter::has_errors() {
  return has_errors_;
}
