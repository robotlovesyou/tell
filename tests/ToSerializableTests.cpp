#include <iostream>
#include "catch2/catch.hpp"
#include "../til/TestErrorReporter.h"
#include "../til/Parser.h"
#include "../til/StringCursor.h"
#include "../til/SerializableAST.h"

TEST_CASE("AST.ToSerializable") {
  const char *source = R"SOURCE(
/// This is a comment about the directive
/// which is continued here
!a_directive "the directive value"

/// This is a comment about Empty
message Empty {
}

/// This is a comment about AMessage
message AMessage {
  /// a_field comment
  a_field: int
  another_field: Empty
  yet_another_field: list[Empty]
  one_last_field: map[Empty]
}

/// This is a comment about TestService
service TestService {
  /// This is a comment about test_call
  test_call[Empty]: AMessage
}
)SOURCE";

  auto er = std::make_shared<til::TestErrorReporter>();
  auto cursor = std::make_unique<til::StringCursor>(source);
  auto tl = std::make_unique<til::Lexer>(std::move(cursor), er);

  til::Parser p(std::move(tl), er);
  auto ast = p.Parse();
  auto sast = ast->ToSerializable();
  CHECK(sast.directive_declarations.size()==1);
  CHECK(sast.directive_declarations[0]->name=="a_directive");
  CHECK(sast.directive_declarations[0]->doc_comment=="This is a comment about the directive\nwhich is continued here");

  CHECK(sast.message_declarations.size()==2);
  CHECK(sast.message_declarations[0]->name=="Empty");
  CHECK(sast.message_declarations[0]->doc_comment=="This is a comment about Empty");
  CHECK(sast.message_declarations[0]->fields.empty());

  CHECK(sast.message_declarations[1]->name=="AMessage");
  CHECK(sast.message_declarations[1]->doc_comment=="This is a comment about AMessage");
  CHECK(sast.message_declarations[1]->fields.size()==4);
  CHECK(sast.message_declarations[1]->fields[0]->doc_comment=="a_field comment");
  CHECK(sast.message_declarations[1]->fields[0]->name=="a_field");
  CHECK(sast.message_declarations[1]->fields[0]->type_def->t()==til::TypeDef::kScalar);
  CHECK(sast.message_declarations[1]->fields[1]->name=="another_field");
  CHECK(sast.message_declarations[1]->fields[1]->type_def->t()==til::TypeDef::kMessage);
  CHECK(sast.message_declarations[1]->fields[2]->name=="yet_another_field");
  CHECK(sast.message_declarations[1]->fields[2]->type_def->t()==til::TypeDef::kList);
  CHECK(sast.message_declarations[1]->fields[3]->name=="one_last_field");
  CHECK(sast.message_declarations[1]->fields[3]->type_def->t()==til::TypeDef::kMap);

  CHECK(sast.service_declarations.size()==1);
  CHECK(sast.service_declarations[0]->doc_comment=="This is a comment about TestService");
  CHECK(sast.service_declarations[0]->name=="TestService");
  CHECK(sast.service_declarations[0]->calls.size()==1);
  CHECK(sast.service_declarations[0]->calls[0]->doc_comment=="This is a comment about test_call");
  CHECK(sast.service_declarations[0]->calls[0]->name=="test_call");
  CHECK(sast.service_declarations[0]->calls[0]->argument=="Empty");
  CHECK(sast.service_declarations[0]->calls[0]->returns=="AMessage");

  json j(sast);
  std::cout << "Pretty Printed ast.ToSerializable()" << std::endl;
  std::cout << j.dump(4) << std::endl;

}
