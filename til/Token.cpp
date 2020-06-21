#include "Token.h"
#include "fmt/core.h"

namespace til {

std::string Token::TypeName() const {
  switch (t) {
    case kString:return "kString";
    case kDocString:return "kDocString";
    case kIdent:return "kIdent";
    case kBang:return "kBang";
    case kColon:return "kColon";
    case kQMark:return "kQMark";
    case kLBrace:return "kLBrace";
    case kRBrace:return "kRBrace";
    case kLSqBracket:return "kLSqBracket";
    case kRSqBracket:return "kRSqBracket";
    case kMessage:return "kMessage";
    case kList:return "kList";
    case kMap:return "kMap";
    case kService:return "kService";
    case kFloat:return "kFloat";
    case kInt:return "kInt";
    case kBool:return "kBool";
    case kStringWord:return "kStringWord";
    case kTime:return "kTime";
    case kLineFeed: return "kLineFeed";
    case kEOF: return "kEOF";
  }
}
std::string Token::Literal() const {
  switch (t) {
    case kString:return fmt::format("\"{}\"", repr);
    case kDocString:return fmt::format("/// {}", repr);
    case kIdent:
    case kBang:
    case kColon:
    case kQMark:
    case kLBrace:
    case kRBrace:
    case kLSqBracket:
    case kRSqBracket:return repr;
    case kMessage:return kKeywordMessage;
    case kList:return kKeywordList;
    case kMap: return kKeywordMap;
    case kService:return kKeywordService;
    case kFloat:return kKeywordFloat;
    case kInt:return kKeywordInt;
    case kBool:return kKeywordBool;
    case kStringWord:return kKeywordString;
    case kTime:return kKeywordTime;
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

