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
using directive_parser = std::function<void(std::unique_ptr<til::DocCommentContext>)>;
using type_def_parser = std::function<std::unique_ptr<til::TypeDef>()>;

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
  std::map<Token::Type, type_def_parser> type_def_parsers_;

  /**
   * Top Level Parsing Functions
   */
  void ParseDirective(std::unique_ptr<til::DocCommentContext> doc);
  void ParseMessage(std::unique_ptr<til::DocCommentContext> doc);
  void ParseService(std::unique_ptr<til::DocCommentContext> doc);
  std::unique_ptr<DocCommentContext> ParseDocComment();
  void ConsumeEOF();

  /**
   * Field Parsing Functions
   */
  std::unique_ptr<TypeDef> ParseTypeDef();
  std::unique_ptr<TypeDef> ParseScalarTypeDef();
  std::unique_ptr<TypeDef> ParseMessageTypeDef();
  std::unique_ptr<TypeDef> ParseMapTypeDef();
  std::vector<std::unique_ptr<Field>> ParseMessageFields();
  std::unique_ptr<Field> ParseField(std::unique_ptr<DocCommentContext> doc);

  /**
   * Helper Functions
   */
  std::optional<const Token*> ConsumeLineFeed();
  void ExpectPeek(Token::Type t);
  til::Token ExpectPeekConsume(Token::Type t);
  void ConsumePastNext(Token::Type t);
  void HandleParsingError(const ParsingException &err, Token::Type consume_past);
  void HandleUnexpectedTopLevelToken();
  bool TypeIsOptional();
};
}

#endif //TELL_COMPILER_PARSER_H_
