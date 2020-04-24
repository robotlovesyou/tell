#include "Parser.h"

#include <optional>
#include <utility>
#include <iostream>

unique_tkn to_unique_tkn(til::Token tkn) {
  return std::unique_ptr<til::Token>(new til::Token(std::move(tkn))); // NOLINT(modernize-make-unique)
}

til::Parser::Parser(std::unique_ptr<til::Lexer> lexer, std::shared_ptr<til::ErrorReporter> error_reporter)
    : lexer_(std::move(lexer)), error_reporter_(std::move(error_reporter)) {
  ast_ = std::make_shared<AST>();

  // Prepare the map of top level directive parsing functions
  declaration_parsers_[Token::kBang] = [this]() { this->ParseDirective(); };
  declaration_parsers_[Token::kMessage] = [this]() { this->ParseMessage(); };
  declaration_parsers_[Token::kService] = [this]() { this->ParseService(); };
}

std::shared_ptr<til::AST> til::Parser::Parse() {
  while (std::optional<const til::Token *> otkn = lexer_->Peek()) {
    ConsumeLineFeeds();
    current_doc_ = std::make_unique<til::DocCommentContext>();

    if (declaration_parsers_.count((**otkn).t)==0) {
      std::cout << "Got token: " << (**otkn).TypeName() << std::endl;
      lexer_->Next();
      continue;
    } else {
      auto parse_fn = declaration_parsers_[(**otkn).t];
      parse_fn();
    }
  }

  return ast_;
}

void til::Parser::ParseDirective() {
  auto start_tkn = to_unique_tkn(std::move(*lexer_->Next()));
  // TODO: Create an expect_peek which checks the type of the next token and throws an error if it is not the expected type
  // TODO: Check for and then consume the ident token;
  // TODO: Check for an then consume the string token;
  // TODO: Check for and then consume (and discard) the newline token;
  // TODO: Create the directive declaration and add it to the ast.
  std::cout << "Parsing directive" << std::endl;
}

void til::Parser::ParseMessage() {
  auto tkn = to_unique_tkn(std::move(*lexer_->Next()));
  std::cout << "Parsing message" << std::endl;
}

void til::Parser::ParseService() {
  auto tkn = to_unique_tkn(std::move(*lexer_->Next()));
  std::cout << "Parsing service>" << std::endl;
}

void til::Parser::ConsumeLineFeeds() {
  while (std::optional<const Token *> opeek = lexer_->Peek()) {
    if ((**opeek).t!=til::Token::kLineFeed) {
      break;
    }
    lexer_->Next();
  }
}

