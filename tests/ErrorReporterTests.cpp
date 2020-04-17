#include "catch2/catch.hpp"
#include "../compiler/ErrorReporter.h"

TEST_CASE("ErrorReporter no errors") {
  til::ErrorReporter er;
  CHECK_FALSE(er.has_errors());
}

TEST_CASE("ErrorReporter.ReportError") {
  til::ErrorReporter er;
  er.ReportError("This is not really an error. It is output from a test");
  CHECK(er.has_errors());
}

TEST_CASE("ErrorReporter.ReportWarning") {
  til::ErrorReporter er;
  er.ReportWarning("This is not really a warning. It is output from a test");
  CHECK_FALSE(er.has_errors());
}
