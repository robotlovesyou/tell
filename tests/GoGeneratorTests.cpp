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

TEST_CASE("GoGenerator outputs the correct package name") {
  SECTION("Simple package name") {
    std::string source = R"SOURCE(
!go_path "a_path"
!go_package "a_package"
)SOURCE";
    auto ast = compile_to_serializable(source);
    gen::GoGenerator generator;
    auto result = generator.Generate(ast);
    std::cout << "Output for Simple package name test" << std::endl;
    std::cout << result << std::endl;
    CHECK(result.find("package a_package") != std::string::npos);
  }

  SECTION("url package name") {
    std::string source = R"SOURCE(
!go_path "github.com/tell/a_package"
!go_package "a_package"
)SOURCE";
    auto ast = compile_to_serializable(source);
    gen::GoGenerator generator;
    auto result = generator.Generate(ast);
    std::cout << "Output for url package name test" << std::endl;
    std::cout << result << std::endl;
    CHECK(result.find("package a_package") != std::string::npos);
  }
}

TEST_CASE("GoGenerator outputs correct structure for a message") {
  std::string source = R"SOURCE(
!go_path "a_path"
!go_package "a_package"

/// Inner is the inner struct
message Inner {
}

/// Outer is the outer struct
message Outer {
  field_int: int
  /// FieldOptionalInt has a doc comment
  field_optional_int: int?
  field_float: float
  field_bool: bool
  field_string: string
  field_time: time
  field_inner: Inner
  field_list: list[Inner]
  field_map: map[Inner]
  field_complex: list[map[Inner?]]
}
)SOURCE";

  std::string expected = R"SOURCE(package a_package

import (
  "time"
)

var _ = time.Now() // Prevent import errors

// Inner is the inner struct
type Inner struct {
}

// Outer is the outer struct
type Outer struct {
  FieldInt int64 `json:"field_int,omitempty"`
  // FieldOptionalInt has a doc comment
  FieldOptionalInt *int64 `json:"field_optional_int,omitempty"`
  FieldFloat float64 `json:"field_float,omitempty"`
  FieldBool bool `json:"field_bool,omitempty"`
  FieldString string `json:"field_string,omitempty"`
  FieldTime time.Time `json:"field_time,omitempty"`
  FieldInner Inner `json:"field_inner,omitempty"`
  FieldList []Inner `json:"field_list,omitempty"`
  FieldMap map[string]Inner `json:"field_map,omitempty"`
  FieldComplex []map[string]*Inner `json:"field_complex,omitempty"`
}

)SOURCE";

  auto ast = compile_to_serializable(source);
  gen::GoGenerator generator;
  auto result = generator.Generate(ast);
  std::cout << "Output for struct test" << std::endl;
  std::cout << result << std::endl;
  CHECK(result == expected);
}
