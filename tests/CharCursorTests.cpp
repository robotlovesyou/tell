#include "catch2/catch.hpp"
#include "../compiler/StringCursor.h"

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

TEST_CASE("Multi character Source") {
  const char* input = "abcdefghijk";
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

