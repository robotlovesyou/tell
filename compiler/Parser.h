//
// Created by ANDREW SMITH on 24/04/2020.
//

#ifndef TELL_COMPILER_PARSER_H_
#define TELL_COMPILER_PARSER_H_

#include <functional>
#include <map>
#include "AST.h"
#include "Declaration.h"
#include "DirectiveDeclaration.h"
#include "DocCommentContext.h"
#include "MessageDeclaration.h"
#include "ParsingException.h"
#include "ServiceDeclaration.h"

#include "Lexer.h"

using unique_doc = std::unique_ptr<til::DocCommentContext>;
using unique_tkn = std::unique_ptr<til::Token>;
using directive_parser = std::function<void()>;

namespace til {
class Parser {
 public:

  Parser(std::unique_ptr<til::Lexer> lexer, std::shared_ptr<ErrorReporter> error_reporter);
  std::shared_ptr<AST> Parse();
 private:
  std::unique_ptr<Lexer> lexer_;
  std::shared_ptr<ErrorReporter> error_reporter_;
  std::shared_ptr<AST> ast_;
  std::map<Token::Type, directive_parser> top_level_parsers_;
  unique_doc current_doc_;

  /**
   * Top Level Parsing Functions
   */
  void ParseDirective();
  void ParseMessage();
  void ParseService();
  void ParseDocComment();
  void ConsumeLineFeeds();
  void ConsumeEOF();

  /**
   * Helper Functions
   */
  void ExpectPeek(Token::Type t);
  void ConsumePastNext(Token::Type t);
  void HandleParsingError(const ParsingException &err, Token::Type consume_past);
  void HandleUnexpectedTopLevelToken();
};
}

#endif //TELL_COMPILER_PARSER_H_
