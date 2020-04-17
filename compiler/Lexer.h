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

/**
 * Implements Cursor<Token>
 */
class Lexer : public Cursor<Token> {
 public:
  /**
   * Construct a lexer which consumes from the provided cursor and reports errors to
   * the provided error reporter
   * @param cursor
   * @param error_reporter
   */
  Lexer(std::unique_ptr<Cursor<char>> cursor, std::shared_ptr<ErrorReporter> error_reporter);

  /**
   * Return the next token.
   * @return
   */
  std::optional<Token> Next();

  /**
   * Peek at the next token to be returned
   * @return
   */
  std::optional<const Token *> Peek();

  // explicitly delete copy constructor and copy assignment operator, move constructor and move assignment operator
  // to encourage use of a unique_ptr
  // The default destructor is OK for this class.
  Lexer(const Lexer &) = delete;
  Lexer &operator=(const Lexer &) = delete;
  Lexer(Lexer &&) = delete;
  Lexer &operator=(Lexer &&) = delete;

 private:
  /**
   * Pull characters from the cursor to construct the next token and
   * place it in the next_ member.
   * If it encounters an error then it reports it and then consumes the rest of the line before resuming lexing.
   */
  void read_next_token();

  /**
   * Handle an unexpected character from the stream by reporting it and disregarding the rest of the line
   * @param c
   */
  void handle_unexpected(char c);

  /**
   * Try to consume a string token. If an error is encountered report it and disregard the rest of the line
   * @return
   */
  std::optional<Token> try_read_string_token();

  /**
   * Try to consume a docstring token.
   * May encounter a standard comment instead, in which case it is disregarded
   * @return
   */
  std::optional<Token> try_read_docstring_token();

  /**
   * Try to read an ident token. If the ident is a keyword then return the appropriate keyword token instead
   * @param first
   * @return
   */
  std::optional<Token> try_read_ident_token(char first);

  /**
   * Return true if peeking at the cursor returns the provided character
   * @param c
   * @return
   */
  bool expect_peek(char c);

  /**
   * Consume and disregard whitespace other than the line feed.
   */
  void consume_nbsp();

  /**
   * Consume and disregard all characters up to an including the next linefeed.
   */
  void consume_remaining_line();

  /**
   * Return true if the provided character is regarded as non-breaking whitespace
   * @param c
   * @return
   */
  static bool is_nbsp(char c);

  /**
   * Return true if the provided character is valid as the first character in an ident or keyword
   * @param c
   * @return
   */
  static bool is_ident_start_char(char c);

  /**
   * Return true if the provided character is valid in any position in an ident or keyword
   * @param c
   * @return
   */
  static bool is_ident_char(char c);

  /**
   * Read the next character from the cursor.
   * If the character is a cursor then reset the column value and advance the line value,
   * otherwise just advance the column value.
   * @return
   */
  std::optional<char> read_next_char();

  /**
   * Report an error including the current line and column to the error reporter
   * @param message
   */
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
