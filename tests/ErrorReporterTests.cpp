#include "catch2/catch.hpp"
#include "TestErrorReporter.h"
#include "ConsoleErrorReporter.h"

TEST_CASE("SilentErrorReporter no errors") {
  til::TestErrorReporter er;
  CHECK_FALSE(er.has_errors());
}

TEST_CASE("SilentErrorReporter.ReportError") {
  til::TestErrorReporter er;
  er.ReportError("This is not really an error. It is output from a test");
  CHECK(er.has_errors());
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
