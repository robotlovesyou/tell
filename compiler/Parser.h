//
// Created by ANDREW SMITH on 24/04/2020.
//

#ifndef TELL_COMPILER_PARSER_H_
#define TELL_COMPILER_PARSER_H_

#include "Lexer.h"
#include "AST.h"
#include "ErrorReporter.h"

namespace til {
class Parser {
 public:
  Parser(std::unique_ptr<til::Lexer> lexer, std::shared_ptr<ErrorReporter> error_reporter);
  std::shared_ptr<AST> Parse();
};
}

#endif //TELL_COMPILER_PARSER_H_
