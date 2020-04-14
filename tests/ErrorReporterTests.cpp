#include "catch2/catch.hpp"
#include "../compiler/ErrorReporter.h"

TEST_CASE("ErrorReporter no errors") {
  til::ErrorReporter er;
  CHECK_FALSE(er.has_errors());
}

TEST_CASE("ErrorReporter.ReportError") {
  til::ErrorReporter er;
  er.ReportError("Ignore me, I'm from the ErrorReporter test");
  CHECK(er.has_errors());
}

TEST_CASE("ErrorReporter.ReportWarning") {
  til::ErrorReporter er;
  er.ReportWarning("Ignore me, I'm from the ErrorReporter test");
  CHECK_FALSE(er.has_errors());
}
