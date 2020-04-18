#include "catch2/catch.hpp"
#include "../compiler/SilentErrorReporter.h"
#include "../compiler/ConsoleErrorReporter.h"

TEST_CASE("SilentErrorReporter no errors") {
  til::SilentErrorReporter er;
  CHECK_FALSE(er.has_errors());
}

TEST_CASE("SilentErrorReporter.ReportError") {
  til::SilentErrorReporter er;
  er.ReportError("This is not really an error. It is output from a test");
  CHECK(er.has_errors());
}

TEST_CASE("SilentErrorReporter.ReportWarning") {
  til::SilentErrorReporter er;
  er.ReportWarning("This is not really a warning. It is output from a test");
  CHECK_FALSE(er.has_errors());
}

TEST_CASE("ConsoleErrorReporter no errors") {
  til::ConsoleErrorReporter er;
  CHECK_FALSE(er.has_errors());
}

TEST_CASE("ConsoleErrorReporter.ReportError") {
  til::ConsoleErrorReporter er;
  er.ReportError("This is not really an error. It is output from a test");
  CHECK(er.has_errors());
}

TEST_CASE("ConsoleErrorReporter.ReportWarning") {
  til::ConsoleErrorReporter er;
  er.ReportWarning("This is not really a warning. It is output from a test");
  CHECK_FALSE(er.has_errors());
}
