#import "catch2/catch.hpp"
#import <memory>
#import "GoGenerator.h"
#import "MissingDirectiveException.h"
#import "TestErrorReporter.h"
#import "StringCursor.h"
#import "Lexer.h"
#import "Parser.h"
#import "SerializableAST.h"

til::SerializableAST compile_to_serializable(std::string &source) {
  auto error_reporter = std::make_shared<til::TestErrorReporter>();
  auto cursor = std::make_unique<til::StringCursor>(source);
  auto lexer = std::make_unique<til::Lexer>(std::move(cursor), error_reporter);
  auto parser = std::make_unique<til::Parser>(std::move(lexer), error_reporter);
  auto ast = parser->Parse();
  if (error_reporter->has_errors()) {
    throw std::runtime_error("Source caused compilation errors");
  }
  return ast->ToSerializable();
}

TEST_CASE("An AST without the correct directives throws an error") {
  SECTION("No go_path directive") {
    std::string source;
    auto ast = compile_to_serializable(source);
    gen::GoGenerator generator;
    CHECK_THROWS_AS(generator.Generate(ast), gen::MissingDirectiveException);
  }

  SECTION("No go_package directive") {
    std::string source = "!go_path \"a_path\"\n";
    auto ast = compile_to_serializable(source);
    gen::GoGenerator generator;
    CHECK_THROWS_AS(generator.Generate(ast), gen::MissingDirectiveException);
  }

  SECTION("go_package and go_path directives") {
    std::string source = R"SOURCE(
!go_path "a_path"
!go_package "a_package"
)SOURCE";
    auto ast = compile_to_serializable(source);
    gen::GoGenerator generator;
    CHECK_NOTHROW(generator.Generate(ast));
  }
}
