//
// Created by ANDREW SMITH on 14/04/2020.
//

#include "Lexer.h"
til::Lexer::Lexer(std::unique_ptr<Cursor<char>> cursor, std::shared_ptr<ErrorReporter> error_reporter): cursor_(std::move(cursor)), error_reporter_(std::move(error_reporter)), eof_(false) {
  read_next_token();
}

std::optional<til::Token> til::Lexer::Next() {
  if (eof_) {
    return std::optional<Token>();
  }
  auto out = std::move(next_);
  eof_ = out.t == Token::kEOF;
  read_next_token();
  return out;
}

std::optional<const til::Token *> til::Lexer::Peek() {
  if(eof_) {
    return std::optional<const Token *>();
  }
  return std::optional<const Token*>(&next_);
}

void til::Lexer::read_next_token() {
  if (eof_) {
    return;
  }

  std::optional<Token> otoken;
  while(!otoken) {
    consume_nbsp();
    auto oc = read_next_char();

    if (!oc) {
      // if the option is empty then the next token is EOF.
      otoken = Token{Token::kEOF, line_, column_, ""};
    } else if (*oc == '"') {
      otoken = try_read_string_token();
    } else if (*oc == '/' && expect_peek('/')) {
      otoken = try_read_docstring_token();
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

void til::Lexer::consume_remaining_line() {
  auto onext = read_next_char();
  while(onext && *onext != '\n') {
    onext = read_next_char();
  }
}

void til::Lexer::handle_unexpected(char c) {
  report_error(fmt::format("Unexpected character {}", c));
  consume_remaining_line();
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

std::optional<til::Token> til::Lexer::try_read_docstring_token() {
  int start_col = column_;
  int start_line = line_;

  read_next_char(); // consume the second slash
  if(!expect_peek('/')) {
    // This is an ordinary comment. Consume the rest of the line and return an empty option
    consume_remaining_line();
    return std::optional<til::Token>();
  }

  // token is a docstring so build it.
  read_next_char(); // consume the third slash
  std::string repr;

  auto oc = read_next_char();
  while (oc) {
    if (*oc == '\n') {
      break;
    }
    if (*oc == '\r') { // ignore carriage return
      continue;
    }
    repr += *oc;
    oc = read_next_char();
  }

  return std::optional<til::Token>(til::Token{Token::kDocString, start_line, start_col, repr});
}

bool til::Lexer::expect_peek(char c) {
  auto peeked = cursor_->Peek();
  return peeked && **peeked == c;
}

