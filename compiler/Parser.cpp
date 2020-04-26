#include "Parser.h"
#include "fmt/core.h"

#include <optional>
#include <utility>
#include <iostream>

#include "ScalarTypeDef.h"

unique_tkn to_unique_tkn(til::Token tkn) {
  return std::unique_ptr<til::Token>(new til::Token(std::move(tkn))); // NOLINT(modernize-make-unique)
}

til::Parser::Parser(std::unique_ptr<til::Lexer> lexer, std::shared_ptr<til::ErrorReporter> error_reporter)
    : lexer_(std::move(lexer)), error_reporter_(std::move(error_reporter)) {
  ast_ = std::make_shared<AST>();

  // Prepare the map of top level directive parsing functions
  top_level_parsers_[Token::kBang] =
      [this](std::unique_ptr<til::DocCommentContext> doc) { this->ParseDirective(std::move(doc)); };

  top_level_parsers_[Token::kMessage] =
      [this](std::unique_ptr<til::DocCommentContext> doc) { this->ParseMessage(std::move(doc)); };

  top_level_parsers_[Token::kService] =
      [this](std::unique_ptr<til::DocCommentContext> doc) { this->ParseService(std::move(doc)); };

  type_def_parsers_[Token::kBool] = [this]() { return this->ParseScalarTypeDef(); };
  type_def_parsers_[Token::kFloat] = [this]() { return this->ParseScalarTypeDef(); };
  type_def_parsers_[Token::kInt] = [this]() { return this->ParseScalarTypeDef(); };
  type_def_parsers_[Token::kStringWord] = [this]() { return this->ParseScalarTypeDef(); };
  type_def_parsers_[Token::kTime] = [this]() { return this->ParseScalarTypeDef(); };
}

std::shared_ptr<til::AST> til::Parser::Parse() {
  while (std::optional<const til::Token *> otkn = lexer_->Peek()) {

    otkn = ConsumeLineFeeds();
    if (!otkn.has_value()) {
      break;
    }

    // Create a doc comment context and parse the next docstrings into it.
    auto doc = ParseDocComment();
    otkn = lexer_->Peek();
    if (!otkn.has_value()) {
      break;
    }

    if ((**otkn).t==Token::kEOF) {
      ConsumeEOF();
      break;
    }

    if (top_level_parsers_.count((**otkn).t)==0) {
      HandleUnexpectedTopLevelToken();
    } else {
      top_level_parsers_[(**otkn).t](std::move(doc));
    }
  }

  return ast_;
}

void til::Parser::ParseDirective(std::unique_ptr<til::DocCommentContext> doc) {
  try {
    auto start_tkn = *lexer_->Next();

    auto identTkn = ExpectPeekConsume(Token::kIdent);

    auto valueTkn = ExpectPeekConsume(Token::kString);

    ExpectPeekConsume(Token::kLineFeed);

    ast_->AddDeclaration(std::make_unique<DirectiveDeclaration>(to_unique_tkn(std::move(start_tkn)),
                                                                std::move(doc),
                                                                identTkn.repr,
                                                                valueTkn.repr,
                                                                ast_));

  } catch (const ParsingException &e) {
    // consume until a linefeed or file end
    HandleParsingError(e, Token::kLineFeed);
  }

}

void til::Parser::ParseMessage(std::unique_ptr<til::DocCommentContext> doc) {
  try {
    auto start_tkn = *lexer_->Next();

    auto name_tkn = ExpectPeekConsume(Token::kIdent);

    ExpectPeekConsume(Token::kLBrace);

    ExpectPeek(Token::kLineFeed);
    ConsumeLineFeeds();

    auto fields = ParseMessageFields();

    ExpectPeekConsume(Token::kRBrace);
    ExpectPeekConsume(Token::kLineFeed);

    ast_->AddDeclaration(std::make_unique<MessageDeclaration>(to_unique_tkn(std::move(start_tkn)),
                                                              std::move(doc),
                                                              name_tkn.repr,
                                                              std::move(fields),
                                                              ast_));

  } catch (const ParsingException &e) {
    HandleParsingError(e, Token::kRBrace);
  }

}

void til::Parser::ParseService(std::unique_ptr<til::DocCommentContext> doc) {
  auto tkn = to_unique_tkn(std::move(*lexer_->Next()));
  std::cout << "Parsing service>" << std::endl;

  // TODO: Remember to fail on unconsumed doc comments (ie before the closing brace of the service)
}

std::optional<const til::Token *> til::Parser::ConsumeLineFeeds() {
  while (std::optional<const Token *> opeek = lexer_->Peek()) {
    if ((**opeek).t!=til::Token::kLineFeed) {
      break;
    }
    lexer_->Next();
  }
  return lexer_->Peek();
}

void til::Parser::ExpectPeek(til::Token::Type t) {
  auto opeek = lexer_->Peek();
  if (!opeek) {
    throw ParsingException("Unexpected end of token stream");
  }
  if (!opeek.has_value() || (**opeek).t!=t) {
    auto tkn = *lexer_->Next();
    Token expected_fake{t, 1, 1, ""};
    std::string literal = tkn.t==Token::Type::kLineFeed ? "[LineFeed]" : tkn.Literal();
    throw ParsingException(fmt::format("Unexpected {} \"{}\" at line {} column {} when expecting a {}",
                                       tkn.TypeName(),
                                       literal,
                                       tkn.line,
                                       tkn.col,
                                       expected_fake.TypeName()));
  }
}

til::Token til::Parser::ExpectPeekConsume(til::Token::Type t) {
  ExpectPeek(t);
  return *lexer_->Next();
}

void til::Parser::ConsumePastNext(til::Token::Type t) {
  while (std::optional<Token> onext = lexer_->Next()) {
    if ((*onext).t==t || (*onext).t==Token::kEOF) {
      break;
    }
  }
}

void til::Parser::ConsumeEOF() {
  lexer_->Next();
}

std::unique_ptr<til::DocCommentContext> til::Parser::ParseDocComment() {
  auto doc = std::make_unique<DocCommentContext>();
  while (std::optional<const Token *> opeek = lexer_->Peek()) {
    if ((**opeek).t==Token::kDocString) {
      auto tkn = *lexer_->Next();
      doc->append(tkn.repr);
    } else if ((**opeek).t==Token::kLineFeed) {
      lexer_->Next();
    } else {
      break;
    }
  }
  return doc;
}

void til::Parser::HandleParsingError(const ParsingException &err, til::Token::Type consume_past) {
  ConsumePastNext(consume_past);
  error_reporter_->ReportError(err.what());
}

void til::Parser::HandleUnexpectedTopLevelToken() {
  auto tkn = *lexer_->Next();
  std::string literal = tkn.t==Token::Type::kLineFeed ? "[LineFeed]" : tkn.Literal();
  ParsingException
      pe(fmt::format("Unexpected {} \"{}\" at line {} column {}", tkn.TypeName(), literal, tkn.line, tkn.col));
  HandleParsingError(pe, Token::kLineFeed);
}

std::unique_ptr<til::TypeDef> til::Parser::ParseScalarTypeDef() {
  auto tkn = *lexer_->Next();
  ScalarTypeDef::ScalarType scalar_type;

  switch (tkn.t) {
    case Token::kBool:scalar_type = ScalarTypeDef::kBool;
      break;
    case Token::kFloat:scalar_type = ScalarTypeDef::ScalarType::kFloat;
      break;
    case Token::kInt:scalar_type = ScalarTypeDef::kInt;
      break;
    case Token::kStringWord:scalar_type = ScalarTypeDef::kString;
      break;
    case Token::kTime:scalar_type = ScalarTypeDef::kTime;
      break;
    default:
      throw std::invalid_argument(fmt::format("Got a {} parsing a scalar message type at line {} column {}",
                                              tkn.TypeName(),
                                              tkn.line,
                                              tkn.col));
  }

  bool optional = false;
  auto opeek = lexer_->Peek();
  if (opeek.has_value() && (**opeek).t==Token::kQMark) {
    optional = true;
    lexer_->Next();
  }

  return std::make_unique<ScalarTypeDef>(scalar_type, optional);
}

std::vector<std::unique_ptr<til::Field>> til::Parser::ParseMessageFields() {
  std::vector<std::unique_ptr<Field>> fields;

  while (std::optional<const Token *> otkn = lexer_->Peek()) {
    otkn = ConsumeLineFeeds();
    if (!otkn.has_value()) {
      break;
    }

    if ((**otkn).t==Token::kRBrace) {
      break;
    }

    auto doc = ParseDocComment();
    fields.push_back(ParseField(std::move(doc)));
  }

  return fields;
}

std::unique_ptr<til::Field> til::Parser::ParseField(std::unique_ptr<DocCommentContext> doc) {
  auto ident_tkn = ExpectPeekConsume(Token::kIdent);

  ExpectPeekConsume(Token::kColon);

  auto otkn = lexer_->Peek();
  if (!otkn.has_value()) {
    throw ParsingException("Unexpected end of token stream parsing message field");
  }
  auto peek = *otkn;

  if (type_def_parsers_.count(peek->t)==0) {
    throw ParsingException(fmt::format("Unexpected {} \"{}\" at line {} column {} parsing message field",
                                           peek->TypeName(),
                                           peek->Literal(),
                                           peek->line,
                                           peek->col));
  }
  auto field_type = type_def_parsers_[peek->t]();
  return std::make_unique<Field>(ident_tkn.repr, std::move(field_type), std::move(doc));
}

