#include "catch2/catch.hpp"
#include <tuple>
#include "Parser.h"
#include "StringCursor.h"
#include "TestErrorReporter.h"
#include "ParserTestHelpers.h"
#include "MessageTypeDef.h"
#include "MapTypeDef.h"
#include "ListTypeDef.h"

std::shared_ptr<til::ErrorReporter> test_error_reporter() {
  return std::make_shared<til::TestErrorReporter>();
}

std::unique_ptr<til::Lexer> test_lexer(const char *source, const std::shared_ptr<til::ErrorReporter> &er) {
  auto cursor = std::make_unique<til::StringCursor>(source);
  return std::make_unique<til::Lexer>(std::move(cursor), er);
}

TEST_CASE("Parser constructor") {
  auto er = test_error_reporter();
  CHECK_NOTHROW(til::Parser(test_lexer("", er), er));
}

TEST_CASE("Parser.parse directive") {

  SECTION("Valid directive") {
    const char *source = R"SOURCE(
/// This is a comment about the directive
/// which is continued here
!a_directive "the directive value"
)SOURCE";

    auto er = test_error_reporter();
    auto tl = test_lexer(source, er);

    til::Parser p(std::move(tl), er);
    auto ast = p.Parse();
    CHECK(ast->DeclarationCount()==1);
    CHECK(ast->Declaration(0)->t()==til::Declaration::kDirective);
    const auto *dd = dynamic_cast<const til::DirectiveDeclaration *>(ast->Declaration(0));
    CHECK(dd->doc().content()=="This is a comment about the directive\nwhich is continued here");
    CHECK(dd->name()=="a_directive");
    CHECK(dd->value()=="the directive value");
    CHECK_FALSE(er->has_errors());
  }

  SECTION("No ident") {
    const char *source = R"SOURCE(
!"the directive value"
)SOURCE";
    auto er = test_error_reporter();
    auto tl = test_lexer(source, er);
    til::Parser p(std::move(tl), er);
    auto ast = p.Parse();
    CHECK(ast->DeclarationCount()==0);
    CHECK(er->has_errors());
  }

  SECTION("No value") {
    const char *source = R"SOURCE(
!a_directive
)SOURCE";

    auto er = test_error_reporter();
    auto tl = test_lexer(source, er);
    til::Parser p(std::move(tl), er);
    auto ast = p.Parse();
    CHECK(ast->DeclarationCount()==0);
    CHECK(er->has_errors());
  }

  SECTION("No line feed after directive") {
    const char *source = R"SOURCE(
!a_directive "the directive value")SOURCE";

    auto er = test_error_reporter();
    auto tl = test_lexer(source, er);
    til::Parser p(std::move(tl), er);
    auto ast = p.Parse();
    CHECK(ast->DeclarationCount()==0);
    CHECK(er->has_errors());
  }
}

std::tuple<int, bool, bool, std::string, til::ScalarTypeDef::ScalarType> scalar_test_opt(int idx,
                                                                                         bool optional,
                                                                                         bool has_docs,
                                                                                         const std::string& name,
                                                                                         til::ScalarTypeDef::ScalarType t) {
  return std::tuple<int, bool, bool, std::string, til::ScalarTypeDef::ScalarType>(idx, optional, has_docs, name, t);
}

TEST_CASE("Parser.parse message with no fields") {

  const char *source = R"SOURCE(
message an_empty_message {
}
)SOURCE";

  auto er = test_error_reporter();
  auto tl = test_lexer(source, er);
  til::Parser p(std::move(tl), er);
  auto ast = p.Parse();
  CHECK_FALSE (er->has_errors());
  CHECK(ast->DeclarationCount()==1);
  CHECK(ast->Declaration(0)->t()==til::Declaration::kMessage);
  const auto *md = dynamic_cast<const til::MessageDeclaration *>(ast->Declaration(0));
  CHECK(md->name()=="an_empty_message");

}

TEST_CASE("Parser.parse message with scalar fields") {

  const char *source = R"SOURCE(
/// doc comment for a_scalar_only_message
message a_scalar_only_message {
  a_bool_field: bool

  /// A doc comment
  /// for an optional_bool_field
  an_optional_bool_field: bool?

  a_float_field: float
  an_optional_float_field: float?
  /// A doc comment for an_int_field
  an_int_field: int
  an_optional_int_field: int?
  a_string_field: string
  an_optional_string_field: string?
  /// A doc comment for a_time_field

  a_time_field: time
  an_optional_time_field: time?
}
)SOURCE";

  auto er = test_error_reporter();
  auto tl = test_lexer(source, er);
  til::Parser p(std::move(tl), er);
  auto ast = p.Parse();
  CHECK_FALSE(er->has_errors());
  CHECK(ast->DeclarationCount()==1);
  CHECK(ast->Declaration(0)->t()==til::Declaration::kMessage);
  CHECK(ast->Declaration(0)->doc().has_content());
  const auto *md = dynamic_cast<const til::MessageDeclaration *>(ast->Declaration(0));
  CHECK(md->name()=="a_scalar_only_message");
  CHECK(md->FieldCount()==10);

  auto opts = GENERATE(
      scalar_test_opt(0, false, false, "a_bool_field", til::ScalarTypeDef::kBool),
      scalar_test_opt(1, true, true, "an_optional_bool_field", til::ScalarTypeDef::kBool),
      scalar_test_opt(2, false, false, "a_float_field", til::ScalarTypeDef::kFloat),
      scalar_test_opt(3, true, false, "an_optional_float_field", til::ScalarTypeDef::kFloat),
      scalar_test_opt(4, false, true, "an_int_field", til::ScalarTypeDef::kInt),
      scalar_test_opt(5, true, false, "an_optional_int_field", til::ScalarTypeDef::kInt),
      scalar_test_opt(6, false, false, "a_string_field", til::ScalarTypeDef::kString),
      scalar_test_opt(7, true, false, "an_optional_string_field", til::ScalarTypeDef::kString),
      scalar_test_opt(8, false, true, "a_time_field", til::ScalarTypeDef::kTime),
      scalar_test_opt(9, true, false, "an_optional_time_field", til::ScalarTypeDef::kTime)
  );

  auto idx = std::get<0>(opts);
  auto optional = std::get<1>(opts);
  auto has_docs = std::get<2>(opts);
  auto name = std::get<3>(opts);
  auto scalar_type = std::get<4>(opts);

  INFO(name);
  CHECK(md->Field(idx)->type_def()->t()==til::TypeDef::kScalar);
  CHECK(md->Field(idx)->type_def()->optional()==optional);
  CHECK(md->Field(idx)->name()==name);
  CHECK(md->Field(idx)->doc().has_content()==has_docs);
  const auto *std = dynamic_cast<const til::ScalarTypeDef *>(md->Field(idx)->type_def());
  CHECK(std->scalar_type()==scalar_type);
}

TEST_CASE("Parser.parse message with message fields") {
  const char *source = R"SOURCE(
message Empty {
}

/// A comment for the message with messages
message a_message_with_message_fields {
  /// A comment for the empty field
  an_empty_field: Empty

  /// A comment for the optional empty field
  an_optional_empty_field: Empty?
}
)SOURCE";
  auto er = test_error_reporter();
  auto tl = test_lexer(source, er);
  til::Parser p(std::move(tl), er);
  auto ast = p.Parse();
  CHECK_FALSE(er->has_errors());
  CHECK(ast->DeclarationCount()==2);
  // We only care about the second declaration
  CHECK(ast->Declaration(1)->t()==til::Declaration::kMessage);
  CHECK(ast->Declaration(1)->doc().has_content());
  const auto *md = dynamic_cast<const til::MessageDeclaration*>(ast->Declaration(1));
  CHECK(md->name() == "a_message_with_message_fields");
  CHECK(md->FieldCount() == 2);

  CHECK(md->Field(0)->type_def()->t() == til::TypeDef::kMessage);
  CHECK_FALSE(md->Field(0)->type_def()->optional());
  CHECK(md->Field(0)->name() == "an_empty_field");
  CHECK(md->Field(0)->doc().has_content());
  const auto *msgt = dynamic_cast<const til::MessageTypeDef*>(md->Field(0)->type_def());
  CHECK(msgt->name() == "Empty");

  CHECK(md->Field(1)->type_def()->t() == til::TypeDef::kMessage);
  CHECK(md->Field(1)->type_def()->optional());
  CHECK(md->Field(1)->name() == "an_optional_empty_field");
  CHECK(md->Field(1)->doc().has_content());
  msgt = dynamic_cast<const til::MessageTypeDef*>(md->Field(1)->type_def());
  CHECK(msgt->name() == "Empty");

}

std::tuple<int, std::string, til::TypeDef::Type, bool, bool> sub_type_test_opt(int idx, const std::string& name, til::TypeDef::Type sub_t, bool optional, bool sub_optional) {
  return std::tuple<int, std::string, til::TypeDef::Type, bool, bool>(idx, name, sub_t, optional, sub_optional);
}

TEST_CASE("Parser.parse message with map fields") {
  const char*source = R"SOURCE(
/// This message only exists to be a type for a map
message Sub {
  a_field: bool
}

/// A comment for the message with maps
message a_message_with_map_fields {
  a_map_of_ints_field: map[int]

  /// a doc comment
  an_optional_map_of_ints_field: map[int]?
  a_map_of_optional_ints_field: map[int?]
  an_optional_map_of_optional_ints_field: map[int?]?

  /// another doc comment
  a_map_of_messages_field: map[Sub]
  an_optional_map_of_messages_field: map[Sub]?
  a_map_of_optional_messages_field: map[Sub?]
  an_optional_map_of_optional_messages_field: map[Sub?]?
}
)SOURCE";

  auto er = test_error_reporter();
  auto tl = test_lexer(source, er);
  til::Parser p(std::move(tl), er);
  auto ast = p.Parse();
  CHECK_FALSE(er->has_errors());
  CHECK(ast->DeclarationCount() == 2);
  CHECK(ast->Declaration(1)->t() == til::Declaration::kMessage);
  CHECK(ast->Declaration(1)->doc().has_content());

  const auto *md = dynamic_cast<const til::MessageDeclaration *>(ast->Declaration(1));

  auto opts = GENERATE(
      sub_type_test_opt(0, "a_map_of_ints_field", til::TypeDef::kScalar, false, false),
      sub_type_test_opt(1, "an_optional_map_of_ints_field", til::TypeDef::kScalar, true, false),
      sub_type_test_opt(2, "a_map_of_optional_ints_field", til::TypeDef::kScalar, false, true),
      sub_type_test_opt(3, "an_optional_map_of_optional_ints_field", til::TypeDef::kScalar, true, true),
      sub_type_test_opt(4, "a_map_of_messages_field", til::TypeDef::kMessage, false, false),
      sub_type_test_opt(5, "an_optional_map_of_messages_field", til::TypeDef::kMessage, true, false),
      sub_type_test_opt(6, "a_map_of_optional_messages_field", til::TypeDef::kMessage, false, true),
      sub_type_test_opt(7, "an_optional_map_of_optional_messages_field", til::TypeDef::kMessage, true, true)
      );

  auto idx = std::get<0>(opts);
  auto name = std::get<1>(opts);
  auto sub_t = std::get<2>(opts);
  auto optional = std::get<3>(opts);
  auto sub_optional = std::get<4>(opts);

  CHECK(md->Field(idx)->name() == name);
  const auto *mtd = dynamic_cast<const til::MapTypeDef *>(md->Field(idx)->type_def());
  CHECK(mtd->t() == til::TypeDef::kMap);
  CHECK(mtd->optional() == optional);
  CHECK(mtd->sub_type()->t() == sub_t);
  CHECK(mtd->sub_type()->optional() == sub_optional);

}

TEST_CASE("Parser.parse message with list fields") {
  const char*source = R"SOURCE(
/// This message only exists to be a type for a list
message Sub {
  a_field: bool
}

/// A comment for the message with lists
message a_message_with_list_fields {
  a_list_of_ints_field: list[int]

  /// a doc comment
  an_optional_list_of_ints_field: list[int]?
  a_list_of_optional_ints_field: list[int?]
  an_optional_list_of_optional_ints_field: list[int?]?

  /// another doc comment
  a_list_of_messages_field: list[Sub]
  an_optional_list_of_messages_field: list[Sub]?
  a_list_of_optional_messages_field: list[Sub?]
  an_optional_list_of_optional_messages_field: list[Sub?]?
}
)SOURCE";

  auto er = test_error_reporter();
  auto tl = test_lexer(source, er);
  til::Parser p(std::move(tl), er);
  auto ast = p.Parse();
  CHECK_FALSE(er->has_errors());
  CHECK(ast->DeclarationCount() == 2);
  CHECK(ast->Declaration(1)->t() == til::Declaration::kMessage);
  CHECK(ast->Declaration(1)->doc().has_content());

  const auto *md = dynamic_cast<const til::MessageDeclaration *>(ast->Declaration(1));

  auto opts = GENERATE(
      sub_type_test_opt(0, "a_list_of_ints_field", til::TypeDef::kScalar, false, false),
      sub_type_test_opt(1, "an_optional_list_of_ints_field", til::TypeDef::kScalar, true, false),
      sub_type_test_opt(2, "a_list_of_optional_ints_field", til::TypeDef::kScalar, false, true),
      sub_type_test_opt(3, "an_optional_list_of_optional_ints_field", til::TypeDef::kScalar, true, true),
      sub_type_test_opt(4, "a_list_of_messages_field", til::TypeDef::kMessage, false, false),
      sub_type_test_opt(5, "an_optional_list_of_messages_field", til::TypeDef::kMessage, true, false),
      sub_type_test_opt(6, "a_list_of_optional_messages_field", til::TypeDef::kMessage, false, true),
      sub_type_test_opt(7, "an_optional_list_of_optional_messages_field", til::TypeDef::kMessage, true, true)
  );

  auto idx = std::get<0>(opts);
  auto name = std::get<1>(opts);
  auto sub_t = std::get<2>(opts);
  auto optional = std::get<3>(opts);
  auto sub_optional = std::get<4>(opts);

  CHECK(md->Field(idx)->name() == name);
  const auto *ltd = dynamic_cast<const til::ListTypeDef *>(md->Field(idx)->type_def());
  CHECK(ltd->t() == til::TypeDef::kList);
  CHECK(ltd->optional() == optional);
  CHECK(ltd->sub_type()->t() == sub_t);
  CHECK(ltd->sub_type()->optional() == sub_optional);

}

TEST_CASE("Parser.parse message with nested map/list fields") {
  const char *source = R"SOURCE(
/// This message only exists to be a type for a list or map
message Sub {
  a_field: bool
}

message NestAllTheThings {
  a_list_of_maps: list[map[Sub]]
  an_optional_map_of_optional_lists: map[list[Sub?]?]?
  a_very_deeply_nested_field: list[map[list[map[list[map[int]]]]]]
}
)SOURCE";
  auto er = test_error_reporter();
  auto tl = test_lexer(source, er);
  til::Parser p(std::move(tl), er);
  auto ast = p.Parse();
  CHECK_FALSE(er->has_errors());
  CHECK(ast->DeclarationCount() == 2);
  CHECK(ast->Declaration(1)->t() == til::Declaration::kMessage);
  CHECK_FALSE(ast->Declaration(1)->doc().has_content());
  const auto *md = dynamic_cast<const til::MessageDeclaration *>(ast->Declaration(1));

  SECTION("a list of maps") {
    CHECK(md->Field(0)->name() == "a_list_of_maps");
    const auto *ltd = dynamic_cast<const til::ListTypeDef *>(md->Field(0)->type_def());
    CHECK(ltd->t() == til::TypeDef::kList);
    CHECK_FALSE(ltd->optional());
    CHECK(ltd->sub_type()->t() == til::TypeDef::kMap);
    CHECK_FALSE(ltd->sub_type()->optional());
    const auto *sub = dynamic_cast<const til::MapTypeDef*>(ltd->sub_type());
    CHECK(sub->sub_type()->t() == til::TypeDef::kMessage);
  }

  SECTION("an optional map of optional lists of optional messages") {
    CHECK(md->Field(1)->name() == "an_optional_map_of_optional_lists");
    const auto *mtd = dynamic_cast<const til::MapTypeDef *>(md->Field(1)->type_def());
    CHECK(mtd->t() == til::TypeDef::kMap);
    CHECK(mtd->optional());
    CHECK(mtd->sub_type()->t() == til::TypeDef::kList);
    CHECK(mtd->sub_type()->optional());
    const auto *sub = dynamic_cast<const til::ListTypeDef*>(mtd->sub_type());
    CHECK(sub->sub_type()->t() == til::TypeDef::kMessage);
    CHECK(sub->sub_type()->optional());
  }

  SECTION("a very deeply nested field") {
    CHECK(md->Field(2)->name() == "a_very_deeply_nested_field");
    const auto *td = md->Field(2)->type_def();
    for(int i = 0; i < 3; i++) {
      const auto *ltd = dynamic_cast<const til::ListTypeDef*>(td);
      const auto *mtd = dynamic_cast<const til::MapTypeDef*>(ltd->sub_type());
      td = mtd->sub_type();
    }
    const auto *sd = dynamic_cast<const til::ScalarTypeDef*>(td);
    CHECK(sd->scalar_type() == til::ScalarTypeDef::kInt);
  }
}

TEST_CASE("Parser.parse empty service") {
  const char *source = R"SOURCE(
/// a doc comment for the service
service my_service {
}
)SOURCE";
  auto er = test_error_reporter();
  auto tl = test_lexer(source, er);
  til::Parser p(std::move(tl), er);
  auto ast = p.Parse();
  CHECK_FALSE(er->has_errors());
  CHECK(ast->DeclarationCount() == 1);
  CHECK(ast->Declaration(0)->t() == til::Declaration::kService);
  const auto *sd = dynamic_cast<const til::ServiceDeclaration*>(ast->Declaration(0));
  CHECK(sd->name() == "my_service");
}

TEST_CASE("Parser.parse service with calls") {
  const char *source = R"SOURCE(
message MyArgument {
}

message MyReturn {
}

message MyOtherArgument {
}

message MyOtherReturn {
}

/// a doc comment for the service
service my_service {
  call_one[MyArgument]: MyReturn
  /// a doc comment for the second call
  call_two[MyOtherArgument]: MyOtherReturn
}
)SOURCE";
  auto er = test_error_reporter();
  auto tl = test_lexer(source, er);
  til::Parser p(std::move(tl), er);
  auto ast = p.Parse();
  CHECK_FALSE(er->has_errors());
  CHECK(ast->DeclarationCount() == 5);
  CHECK(ast->Declaration(4)->t() == til::Declaration::kService);
  const auto *sd = dynamic_cast<const til::ServiceDeclaration*>(ast->Declaration(4));
  CHECK(sd->name() == "my_service");
  CHECK(sd->doc().has_content());
  CHECK(sd->CallCount() == 2);
  CHECK_FALSE(sd->Call(0)->doc().has_content());
  CHECK(sd->Call(0)->name() == "call_one");
  CHECK(sd->Call(0)->argument().name() == "MyArgument");
  CHECK(sd->Call(0)->returns().name() == "MyReturn");
  CHECK(sd->Call(1)->doc().has_content());
  CHECK(sd->Call(1)->name() == "call_two");
  CHECK(sd->Call(1)->argument().name() == "MyOtherArgument");
  CHECK(sd->Call(1)->returns().name() == "MyOtherReturn");
}

TEST_CASE("Parser.parse message with repeated field name") {
  const char *source = R"SOURCE(
message my_message {
  duplicate_field: int
  duplicate_field: int
}
)SOURCE";
  auto er = test_error_reporter();
  auto tl = test_lexer(source, er);
  til::Parser p(std::move(tl), er);
  auto ast = p.Parse();
  CHECK(er->has_errors());
}

TEST_CASE("Parser.parse recursive message loop") {
  const char *source = R"SOURCE(
message A {
  opt_field: B?
  map_field: map[B]
  list_field: list[B]
  recursive_field_a: C
}

message B {
  a_field: A
}

message C {
  recursive_field_c: D
}

message D {
  recursive_field_d: A
}
)SOURCE";
  auto er = test_error_reporter();
  auto tl = test_lexer(source, er);
  til::Parser p(std::move(tl), er);
  auto ast = p.Parse();
  CHECK(er->has_errors());
}

TEST_CASE("Parser.parse duplicate directive name") {
  const char *source = R"SOURCE(
!a_duplicate_directive "the directive value"
!a_duplicate_directive "the directive value"
)SOURCE";
  auto er = test_error_reporter();
  auto tl = test_lexer(source, er);
  til::Parser p(std::move(tl), er);
  p.Parse();
  CHECK(er->has_errors());
}

TEST_CASE("Parser.parse duplicate message name") {
  const char *source = R"SOURCE(
message a_duplicate_message {
}
message a_duplicate_message {
}
)SOURCE";
  auto er = test_error_reporter();
  auto tl = test_lexer(source, er);
  til::Parser p(std::move(tl), er);
  p.Parse();
  CHECK(er->has_errors());
}

TEST_CASE("Parser.parse duplicate service name") {
  const char *source = R"SOURCE(
service a_duplicate_service {
}
service a_duplicate_service {
}
)SOURCE";
  auto er = test_error_reporter();
  auto tl = test_lexer(source, er);
  til::Parser p(std::move(tl), er);
  p.Parse();
  CHECK(er->has_errors());
}

TEST_CASE("Parser.parse duplicate call within a service") {
  const char *source = R"SOURCE(
message MyMessage {
}

service my_service {
  my_call[MyMessage]: MyMessage
  my_call[MyMessage]: MyMessage
}
)SOURCE";
  auto er = test_error_reporter();
  auto tl = test_lexer(source, er);
  til::Parser p(std::move(tl), er);
  p.Parse();
  CHECK(er->has_errors());
}

TEST_CASE("Parser.parse unknown message as field type") {
  const char *source = R"SOURCE(
message MyMessage {
  my_field: UnknownMessage
  my_map_field: map[UnknownMessage]
  my_list_field: list[UnknownMessage]
  my_nested_field: list[map[list[UnknownMessage]]]
}
)SOURCE";
  auto er = test_error_reporter();
  auto tl = test_lexer(source, er);
  til::Parser p(std::move(tl), er);
  p.Parse();
  CHECK(er->has_errors());
}

TEST_CASE("Parser.parse unknown message as call argument or return") {
  const char *source = R"SOURCE(
message KnownMessage {
}

service MyService {
  my_call_with_unknown_argument[UnknownMessage]: KnownMessage
  my_call_with_unknown_return[KnownMessage]: UnknownMessage
  my_call_with_unknown_arg_and_return[UknownMessage]: KnownMessage
}
)SOURCE";
  auto er = test_error_reporter();
  auto tl = test_lexer(source, er);
  til::Parser p(std::move(tl), er);
  p.Parse();
  CHECK(er->has_errors());
}

