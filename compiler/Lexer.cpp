//
// Created by ANDREW SMITH on 14/04/2020.
//

#include "Lexer.h"
til::Lexer::Lexer(std::unique_ptr<Cursor<char>> cursor, std::shared_ptr<ErrorReporter> error_reporter): cursor_(std::move(cursor)), error_reporter_(std::move(error_reporter)), eof_(false) {
  next_token();
}

std::optional<til::Token> til::Lexer::Next() {
  if (eof_) {
    return std::optional<Token>();
  }
  auto out = std::move(next_);
  eof_ = out.t == Token::kEOF;
  next_token();
  return out;
}

std::optional<const til::Token *> til::Lexer::Peek() {
  return std::optional<const Token *>();
}

void til::Lexer::next_token() {
  consume_nbsp();
  auto opt_c = read_next_char();
  if (!opt_c) {
    next_ =  std::move(Token{Token::kEOF, line_, column_, ""});
    return;
  }

  std::optional<Token> onext;
  char c = opt_c.value();
  // TODO convert this to if/else so that the peek can be checked too.
  switch(c) {
    case '"':
      onext = try_read_string_token();
      next_ = onext ? std::move(*onext) : std::move(next_);
      break;
    default:
      handle_unexpected(c);
  }
}

void til::Lexer::consume_nbsp() {
  auto peeked = cursor_->Peek();
  while (peeked && is_nbsp(**peeked)) {
    read_next_char(); // consume the whitespace
    peeked = cursor_->Peek();
  }
}

bool til::Lexer::is_nbsp(char c) {
  switch(c) {
    case ' ':
    case '\t':
    case '\r':
      return true;
    default:
      return false;
  }
}

void til::Lexer::try_consume_newline() {
  auto peeked = cursor_->Peek();
  if (peeked && **peeked == '\n') {
    read_next_char();
  }
}

void til::Lexer::handle_unexpected(char c) {
  report_error(fmt::format("Unexpected character {}", c));
  consume_nbsp();
  try_consume_newline();
}

std::optional<til::Token> til::Lexer::try_read_string_token() {
  int start_ = column_;
  std::string repr;
  std::optional<char> oc = read_next_char();
  while(oc) {
    if(*oc == '\n') {
      report_error("Unexpected newline in string");
      return std::optional<Token>();
    }
    if(*oc == '\r') { // ignore carriage return.
      continue;
    }
    if(*oc == '"') {
      return std::optional<Token>(Token{Token::kString, line_, start_, repr});
    }
    repr += *oc;
    oc = read_next_char();
  }
  report_error("Unexpected eof in string");
  return std::optional<Token>();
}

std::optional<char> til::Lexer::read_next_char() {
  auto oc = cursor_->Next();
  if (oc && *oc == '\n') {
    line_ += 1;
    column_ = 0;
  } else if (oc) {
    column_ += 1;
  }
  return oc;
}
void til::Lexer::report_error(const std::string &message) {
  error_reporter_->ReportError(fmt::format("{} at line {} column {}", message, line_, column_));
}

