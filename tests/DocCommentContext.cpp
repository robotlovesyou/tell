#include "catch2/catch.hpp"
#include "../compiler/DocCommentContext.h"

static const char* line_1 = "This is a comment";
static const char* line_2 = "This is more comment";

TEST_CASE("DocCommentContext overrides += to append a new line to the string") {
  SECTION("Empty comment") {
    til::DocCommentContext dc;
    CHECK_NOTHROW(dc += line_1);
  }
  SECTION("Adding a line") {
    til::DocCommentContext dc;
    CHECK_NOTHROW(dc += line_1);
    CHECK_NOTHROW(dc += line_2);
  }
}

TEST_CASE("DocCommentContext overrides * to access current content") {
  SECTION("Single comment") {
    til::DocCommentContext dc;
    dc += line_1;
    CHECK(*dc == line_1);
  }
  SECTION("Multiline comment") {
    til::DocCommentContext dc;
    dc += line_1;
    dc += line_2;
    std::string ml = line_1;
    ml += "\n";
    ml += line_2;
    CHECK(*dc == ml);
  }
}

TEST_CASE("DocCommentContext evaluates as false when empty") {
  til::DocCommentContext dc;
  CHECK_FALSE(dc);
}

TEST_CASE("DocCommentContext evaluates as true when it has content") {
  til::DocCommentContext dc;
  dc += line_1;
  CHECK(dc);
}