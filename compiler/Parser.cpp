#include "Parser.h"

#include <optional>
#include <utility>
#include <iostream>

#include "fmt/core.h"

unique_tkn to_unique_tkn(til::Token tkn) {
  return std::unique_ptr<til::Token>(new til::Token(std::move(tkn))); // NOLINT(modernize-make-unique)
}

til::Parser::Parser(std::unique_ptr<til::Lexer> lexer, std::shared_ptr<til::ErrorReporter> error_reporter)
    : lexer_(std::move(lexer)), error_reporter_(std::move(error_reporter)) {
  ast_ = std::make_shared<AST>();

  // Prepare the map of top level directive parsing functions
  top_level_parsers_[Token::kBang] = [this]() { this->ParseDirective(); };
  top_level_parsers_[Token::kMessage] = [this]() { this->ParseMessage(); };
  top_level_parsers_[Token::kService] = [this]() { this->ParseService(); };
  top_level_parsers_[Token::kDocString] = [this]() { this->ParseDocComment(); };
  top_level_parsers_[Token::kLineFeed] = [this]() { this->ConsumeLineFeeds(); };
  top_level_parsers_[Token::kEOF] = [this]() { this->ConsumeEOF(); };
}

std::shared_ptr<til::AST> til::Parser::Parse() {
  while (std::optional<const til::Token *> otkn = lexer_->Peek()) {

    // If the last doc comment context has been consumed then create a new one
    current_doc_ = current_doc_==nullptr ? std::make_unique<DocCommentContext>() : std::move(current_doc_);

    if (top_level_parsers_.count((**otkn).t)==0) {
      HandleUnexpectedTopLevelToken();
    } else {
      top_level_parsers_[(**otkn).t]();
    }
  }

  return ast_;
}

void til::Parser::ParseDirective() {
  try {
    auto start_tkn = to_unique_tkn(std::move(*lexer_->Next()));

    ExpectPeek(Token::kIdent);
    auto identTkn = *lexer_->Next();

    ExpectPeek(Token::kString);
    auto valueTkn = *lexer_->Next();

    ExpectPeek(Token::kLineFeed);

    ast_->AddDeclaration(std::make_unique<DirectiveDeclaration>(std::move(start_tkn),
                                                                std::move(current_doc_),
                                                                identTkn.repr,
                                                                valueTkn.repr,
                                                                ast_));

  } catch (const ParsingException &e) {
    // consume until a linefeed or file end
    HandleParsingError(e, Token::kLineFeed);
  }

}

void til::Parser::ParseMessage() {
  auto tkn = to_unique_tkn(std::move(*lexer_->Next()));
  std::cout << "Parsing message" << std::endl;

  // TODO: Remember to fail on unconsumed doc comments (ie before the closing brace of the message)
}

void til::Parser::ParseService() {
  auto tkn = to_unique_tkn(std::move(*lexer_->Next()));
  std::cout << "Parsing service>" << std::endl;

  // TODO: Remember to fail on unconsumed doc comments (ie before the closing brace of the service)
}

void til::Parser::ConsumeLineFeeds() {
  while (std::optional<const Token *> opeek = lexer_->Peek()) {
    if ((**opeek).t!=til::Token::kLineFeed) {
      break;
    }
    lexer_->Next();
  }
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

void til::Parser::ParseDocComment() {
  while (std::optional<const Token *> opeek = lexer_->Peek()) {
    if ((**opeek).t==Token::kDocString) {
      auto tkn = *lexer_->Next();
      current_doc_->append(tkn.repr);
    } else if ((**opeek).t==Token::kLineFeed) {
      lexer_->Next();
    } else {
      break;
    }
  }
}

void til::Parser::HandleParsingError(const ParsingException &err, til::Token::Type consume_past) {
  ConsumePastNext(consume_past);
  error_reporter_->ReportError(err.what());
  current_doc_ = nullptr;
}

void til::Parser::HandleUnexpectedTopLevelToken() {
  auto tkn = *lexer_->Next();
  std::string literal = tkn.t==Token::Type::kLineFeed ? "[LineFeed]" : tkn.Literal();
  ParsingException pe(fmt::format("Unexpected {} \"{}\" at line {} column {}", tkn.TypeName(), literal, tkn.line, tkn.col));
  HandleParsingError(pe, Token::kLineFeed);
}

