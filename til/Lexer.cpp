//
// Created by ANDREW SMITH on 14/04/2020.
//

#include "Lexer.h"

std::pair<til::Token::Type, std::string> make_keyword_entry(til::Token::Type t, std::string kw) {
  return std::pair<til::Token::Type, std::string>(t, kw);
}

til::Lexer::Lexer(std::unique_ptr<Cursor<char>> cursor, std::shared_ptr<ErrorReporter> error_reporter)
    : cursor_(std::move(cursor)), error_reporter_(std::move(error_reporter)), eof_(false) {
  keywords_[kKeywordMessage] = make_keyword_entry(Token::kMessage, kKeywordMessage);
  keywords_[kKeywordList] = make_keyword_entry(Token::kList, kKeywordList);
  keywords_[kKeywordMap] = make_keyword_entry(Token::kMap, kKeywordMap);
  keywords_[kKeywordService] = make_keyword_entry(Token::kService, kKeywordService);
  keywords_[kKeywordFloat] = make_keyword_entry(Token::kFloat, kKeywordFloat);
  keywords_[kKeywordInt] = make_keyword_entry(Token::kInt, kKeywordInt);
  keywords_[kKeywordBool] = make_keyword_entry(Token::kBool, kKeywordBool);
  keywords_[kKeywordString] = make_keyword_entry(Token::kStringWord, kKeywordString);
  keywords_[kKeywordTime] = make_keyword_entry(Token::kTime, kKeywordTime);
  read_next_token();
}

std::optional<til::Token> til::Lexer::Next() {
  if (eof_) {
    return std::optional<Token>();
  }
  auto out = std::move(next_);
  eof_ = out.t==Token::kEOF;
  read_next_token();
  return out;
}

std::optional<const til::Token *> til::Lexer::Peek() {
  if (eof_) {
    return std::optional<const Token *>();
  }
  return std::optional<const Token *>(&next_);
}

void til::Lexer::read_next_token() {
  if (eof_) {
    return;
  }

  std::optional<Token> otoken;
  while (!otoken) {
    consume_nbsp();
    auto oc = read_next_char();

    if (!oc) {
      // if the option is empty then the next token is EOF.
      otoken = Token{Token::kEOF, line_, column_, ""};
    } else if (*oc=='"') {
      otoken = try_read_string_token();
    } else if (*oc=='/' && expect_peek('/')) {
      otoken = try_read_docstring_token();
    } else if (is_ident_start_char(*oc)) {
      otoken = try_read_ident_token(*oc);
    } else if (*oc=='!') {
      otoken = Token{Token::kBang, line_, column_, "!"};
    } else if (*oc==':') {
      otoken = Token{Token::kColon, line_, column_, ":"};
    } else if (*oc=='?') {
      otoken = Token{Token::kQMark, line_, column_, "?"};
    } else if (*oc=='{') {
      otoken = Token{Token::kLBrace, line_, column_, "{"};
    } else if (*oc=='}') {
      otoken = Token{Token::kRBrace, line_, column_, "}"};
    } else if (*oc=='[') {
      otoken = Token{Token::kLSqBracket, line_, column_, "["};
    } else if (*oc==']') {
      otoken = Token{Token::kRSqBracket, line_, column_, "]"};
    } else if (*oc=='\n') {
      otoken = read_line_feed_token();
    } else {
      handle_unexpected(*oc);
    }
  }
  next_ = std::move(*otoken);
}

void til::Lexer::consume_nbsp() {
  auto peeked = cursor_->Peek();
  while (peeked && is_nbsp(**peeked)) {
    read_next_char(); // consume the whitespace
    peeked = cursor_->Peek();
  }
}

bool til::Lexer::is_nbsp(char c) {
  switch (c) {
    case ' ':
    case '\t':
    case '\r':return true;
    default:return false;
  }
}

bool til::Lexer::is_ident_start_char(char c) {
  return std::isalpha(c) || c=='_';
}

bool til::Lexer::is_ident_char(char c) {
  return std::isalnum(c) || c=='_';
}

void til::Lexer::consume_remaining_line() {
  while (auto onext = read_next_char()) {
    if (*onext=='\n') {
      break;
    }
  }
}

void til::Lexer::handle_unexpected(char c) {
  report_error(fmt::format("Unexpected character {}", c));
  consume_remaining_line();
}

std::optional<til::Token> til::Lexer::try_read_string_token() {
  int start_ = column_;
  std::string repr;

  while (auto pc = cursor_->Peek()) {
    if (**pc=='\n') {
      report_error("Unexpected newline in string");
      read_next_char();
      consume_remaining_line();
      return std::optional<Token>();
    }
    if (**pc=='\r') { // ignore carriage return.
      read_next_char();
      continue;
    }
    if (**pc=='"') {
      read_next_char();
      return std::optional<Token>(Token{Token::kString, line_, start_, repr});
    }
    repr += *read_next_char();
  }
  report_error("Unexpected eof in string");
  return std::optional<Token>();
}

std::optional<til::Token> til::Lexer::try_read_docstring_token() {
  int start_col = column_;
  int start_line = line_;

  read_next_char(); // consume the second slash
  if (!expect_peek('/')) {
    // This is an ordinary comment. Consume the rest of the line and return an empty option
    consume_remaining_line();
    return std::optional<til::Token>();
  }

  // token is a docstring so build it.
  // consume the third slash and a maximum of one leading space;
  read_next_char();
  if(expect_peek(' ')) {
    read_next_char();
  }

  std::string repr;

  while (auto oc = read_next_char()) {
    if (*oc=='\n') {
      break;
    }
    if (*oc=='\r') { // ignore carriage return
      continue;
    }
    repr += *oc;
  }

  return std::optional<til::Token>(til::Token{Token::kDocString, start_line, start_col, repr});
}

std::optional<til::Token> til::Lexer::try_read_ident_token(char first) {
  int start = column_;
  std::string repr;
  repr += first;

  while (auto peeked = cursor_->Peek()) {
    if (is_ident_char(**peeked)) {
      repr += *read_next_char();
    } else {
      break;
    }
  }

  if (keywords_.count(repr)==1) {
    auto parts = keywords_[repr];
    return Token{std::get<0>(parts), line_, start, std::get<1>(parts)};
  }
  return til::Token{Token::kIdent, line_, start, repr};
}

std::optional<til::Token> til::Lexer::read_line_feed_token() {
  int start_line = line_at_newline_;
  int start_col = column_at_newline_;

  while(auto peeked = cursor_->Peek()) {
    if(!(is_nbsp(**peeked) || (**peeked) == '\n')) {
      break;
    }
    read_next_char();
  }

  return til::Token{Token::kLineFeed, start_line, start_col, "\n"};
}

std::optional<char> til::Lexer::read_next_char() {
  auto oc = cursor_->Next();
  if (oc && *oc=='\n') {
    line_at_newline_ = line_;
    column_at_newline_ = column_ + 1;
    line_ += 1;
    column_ = 0;
  } else {
    column_ += 1;
  }
  return oc;
}

void til::Lexer::report_error(const std::string &message) {
  error_reporter_->ReportError(fmt::format("{} at line {} column {}", message, line_, column_));
}

bool til::Lexer::expect_peek(char c) {
  auto peeked = cursor_->Peek();
  return peeked && **peeked==c;
}
