#include "catch2/catch.hpp"
#include "../compiler/StringCursor.h"
#include "../compiler/FileCursor.h"

TEST_CASE("StringCursor constructor") {
  CHECK_NOTHROW(til::StringCursor("This is a string"));
}

TEST_CASE("StringCursor empty string") {
  til::StringCursor sc("");
  CHECK_FALSE(sc.Next().has_value());
  CHECK_FALSE(sc.Peek().has_value());
}

TEST_CASE("StringCursor single char") {
  til::StringCursor sc("a");
  auto peek_val = sc.Peek();
  CHECK((peek_val.has_value() && *(peek_val.value()) == 'a'));
  auto val = sc.Next();
  CHECK((val.has_value() && val.value() == 'a'));
  peek_val = sc.Peek();
  CHECK_FALSE(peek_val.has_value());
}

TEST_CASE("Char Cursor Multi character Source") {
  const char* input = "a b c d e f g h i j k";
  til::StringCursor sc(input);
  for (int i = 0; i < strlen(input); i++) {
    auto peek_val = sc.Peek();
    CHECK((peek_val.has_value() && *(peek_val.value()) == input[i]));
    auto val = sc.Next();
    CHECK((val.has_value() && val.value() == input[i]));
  }
  auto val = sc.Next();
  CHECK_FALSE(val.has_value());
}

TEST_CASE("FileCursor constructor") {
  SECTION("File exists") {
    CHECK_NOTHROW(til::FileCursor("CharCursorTestFile.txt"));
  }
  SECTION("File does not exist") {
    CHECK_THROWS_AS(til::FileCursor("nope.txt"), til::FileCursor::FileCursorException);
  }
}

TEST_CASE("FileCursor empty file") {
  til::FileCursor fc("CharCursorTestEmptyFile.txt");
  CHECK_FALSE(fc.Next().has_value());
  CHECK_FALSE(fc.Peek().has_value());
}


TEST_CASE("FileCursor single char") {
  til::FileCursor fc("CharCursorTestSingleCharFile.txt");
  auto peek_val = fc.Peek();
  CHECK((peek_val.has_value() && *(peek_val.value()) == 'a'));
  auto val = fc.Next();
  CHECK((val.has_value() && val.value() == 'a'));
  peek_val = fc.Peek();
  CHECK((peek_val && **peek_val == '\n')); // ifstream adds a termiating newline
}

TEST_CASE("FileCursor multi character") {
  const char* input = "a b c d e f g h i j k\n"; // This must match the contents of the file. ifstream adds a terminating newline.
  til::FileCursor fc("CharCursorTestFile.txt");
  for (int i = 0; i < strlen(input); i++) {
    auto peek_val = fc.Peek();
    CHECK((peek_val.has_value() && *(peek_val.value()) == input[i]));
    auto val = fc.Next();
    CHECK((val.has_value() && val.value() == input[i]));
  }
  auto val = fc.Next();
  CHECK_FALSE(val.has_value());
}

