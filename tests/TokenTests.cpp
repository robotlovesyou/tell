#include "catch2/catch.hpp"
#include "../compiler/Token.h"
#include "fmt/core.h"


TEST_CASE("Token.TypeName", "[Token]") {
  Token tk{Token::kQMark, 0, 0, ""};
  CHECK(tk.TypeName() == "kQMark");
}

TEST_CASE("Token.Literal", "[Token]") {
  SECTION("String") {
    const std::string repr = "A string";
    Token tk{Token::kString, 0, 0, repr};
    CHECK(tk.Literal() == fmt::format("\"{}\"", repr));
  }
  SECTION("DocString") {
    const std::string repr = "A Doc String";
    Token tk{Token::kDocString, 0, 0, repr};
    CHECK(tk.Literal() == fmt::format("/// {}", repr));
  }
  SECTION("Ident etc (repr = literal") {
    const std::string repr = "MyMessage";
    Token tk{Token::kIdent, 0, 0, repr};
    CHECK(tk.Literal() == repr);
  }
  SECTION("Message") {
    Token tk{Token::kMessage, 0, 0, ""};
    CHECK(tk.Literal() == kKeywordMessage);
  }
  SECTION("List") {
    Token tk{Token::kList, 0, 0, ""};
    CHECK(tk.Literal() == kKeywordList);
  }
  SECTION("Service") {
    Token tk{Token::kService, 0, 0, ""};
    CHECK(tk.Literal() == kKeywordService);
  }
  SECTION("Call") {
    Token tk{Token::kCall, 0, 0, ""};
    CHECK(tk.Literal() == kKeywordCall);
  }
}

