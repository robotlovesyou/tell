//
// Created by ANDREW SMITH on 14/04/2020.
//

#include "Lexer.h"
til::Lexer::Lexer(std::unique_ptr<Cursor<char>> cursor, std::shared_ptr<ErrorReporter> error_reporter): cursor_(std::move(cursor)), error_reporter_(std::move(error_reporter)) {
}
std::optional<til::Token> til::Lexer::Next() {
  return std::optional<Token>();
}

std::optional<const til::Token *> til::Lexer::Peek() {
  return std::optional<const Token *>();
}
