//
// Created by ANDREW SMITH on 14/04/2020.
//

#ifndef TELL_COMPILER_LEXER_H_
#define TELL_COMPILER_LEXER_H_

#include <map>
#include <memory>
#include <optional>
#include "Cursor.h"
#include "ErrorReporter.h"
#include "Token.h"

namespace til {
class Lexer : public Cursor<Token> {
 public:
  Lexer(std::unique_ptr<Cursor<char>> cursor, std::shared_ptr<ErrorReporter> error_reporter);
  std::optional<Token> Next();
  std::optional<const Token *> Peek();

  // explicitly delete copy constructor and copy assignment operator, move constructor and move assignment operator
  // to encourage use of a unique_ptr
  // The default destructor is OK for this class.
  Lexer(const Lexer &) = delete;
  Lexer &operator=(const Lexer &) = delete;
  Lexer(Lexer &&) = delete;
  Lexer &operator=(Lexer &&) = delete;

 private:
  void read_next_token();
  void handle_unexpected(char c);
  std::optional<Token> try_read_string_token();
  std::optional<Token> try_read_docstring_token();
  std::optional<Token> try_read_ident_token(char first);
  bool expect_peek(char c);
  bool expect_peek_fn(char c, const std::function<bool(char)>& fn);
  void consume_nbsp();
  void try_consume_newline();
  void consume_remaining_line();
  static bool is_nbsp(char c);
  static bool is_ident_start_char(char c);
  static bool is_ident_char(char c);
  std::optional<char> read_next_char();
  void report_error(const std::string &message);

  std::unique_ptr<Cursor<char>> cursor_;
  std::shared_ptr<ErrorReporter> error_reporter_;
  int line_{1};
  int column_{0};

  // A newline in til is not just whitespace. Keep track of the location of the last seen newline
  // so that its position can be correctly set in the token
  int line_at_newline_{};
  int column_at_newline_{};

  Token next_;
  bool eof_;
  std::map<std::string, std::pair<Token::Type, std::string>> keywords_;
};
}

#endif //TELL_COMPILER_LEXER_H_
