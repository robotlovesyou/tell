//
// Created by ANDREW SMITH on 14/04/2020.
//

#ifndef TELL_COMPILER_LEXER_H_
#define TELL_COMPILER_LEXER_H_

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
  std::unique_ptr<Cursor<char>> cursor_;
  std::shared_ptr<ErrorReporter> error_reporter_;
};
}

#endif //TELL_COMPILER_LEXER_H_
