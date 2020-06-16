#include "catch2/catch.hpp"
#include "../til/DocCommentContext.h"

static const char* line_1 = "This is a comment";
static const char* line_2 = "This is more comment";

TEST_CASE("DocCommentContext.append a new line to the string") {
  SECTION("Empty comment") {
    til::DocCommentContext dc;
    CHECK_NOTHROW(dc.append(line_1));
  }
  SECTION("Adding a line") {
    til::DocCommentContext dc;
    CHECK_NOTHROW(dc.append(line_1));
    CHECK_NOTHROW(dc.append(line_2));
  }
}

TEST_CASE("DocCommentContext.content accesses current content") {
  SECTION("Single comment") {
    til::DocCommentContext dc;
    dc.append(line_1);
    CHECK(dc.content() == line_1);
  }
  SECTION("Multiline comment") {
    til::DocCommentContext dc;
    dc.append(line_1);
    dc.append(line_2);
    std::string ml = line_1;
    ml += "\n";
    ml += line_2;
    CHECK(dc.content() == ml);
  }
}

TEST_CASE("DocCommentContext.has_content is false when empty") {
  til::DocCommentContext dc;
  CHECK_FALSE(dc.has_content());
}

TEST_CASE("DocCommentContext.has_content is true when it has content") {
  til::DocCommentContext dc;
  dc.append(line_1);
  CHECK(dc.has_content());
}