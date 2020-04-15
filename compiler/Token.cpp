//
// Created by ANDREW SMITH on 12/04/2020.
//

#include "Token.h"
#include "fmt/core.h"

namespace til {

std::string Token::TypeName() {
  switch (t) {
    case kString:return "kString";
    case kDocString:return "kDocString";
    case kIdent:return "kIdent";
    case kBang:return "kBang";
    case kColon:return "kColon";
    case kQMark:return "kQMark";
    case kLBrace:return "kLBrace";
    case kRBrace:return "kRBrace";
    case kLBracket:return "kLBracket";
    case kRBracket:return "kRBracket";
    case kMessage:return "kMessage";
    case kList:return "kList";
    case kMap:return "kMap";
    case kService:return "kService";
    case kCall:return "kCall";
    case kLineFeed: return "kLineFeed";
    case kEOF: return "kEOF";
  }
}
std::string Token::Literal() const {
  switch (t) {
    case kString:return fmt::format("\"{}\"", repr);
    case kDocString:return fmt::format("///{}", repr);
    case kIdent:
    case kBang:
    case kColon:
    case kQMark:
    case kLBrace:
    case kRBrace:
    case kLBracket:
    case kRBracket:return repr;
    case kMessage:return kKeywordMessage;
    case kList:return kKeywordList;
    case kMap: return kKeywordMap;
    case kService:return kKeywordService;
    case kCall:return kKeywordCall;
    case kLineFeed: return "\n";
    case kEOF: return "";
  }
}

Token::Token(Token &&source) noexcept {
  t = source.t;
  line = source.line;
  col = source.col;
  repr = std::move(source.repr);
}

Token::Token(Type tp, int l, int c, std::string r): t(tp), line(l), col(c), repr(std::move(r)) {
}

Token &Token::operator=(Token &&source) noexcept {
  t = source.t;
  line = source.line;
  col = source.col;
  repr = std::move(source.repr);
  return *this;
}

} // namespace til

