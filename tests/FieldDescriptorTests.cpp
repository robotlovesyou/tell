#include "catch.hpp"
#include <memory>
#include <optional>
#include "../compiler/FieldDescriptor.h"
#include "../compiler/MessageDescriptor.h"

std::shared_ptr<MessageDescriptor> test_message() {
  return std::make_shared<MessageDescriptor>();
}

std::shared_ptr<FieldDescriptor> test_field(FieldDescriptor::Type t = FieldDescriptor::TYPE_STRING, std::string name = "field_name", std::string doc = "field doc string", std::shared_ptr<MessageDescriptor> md = test_message()) {
  return std::make_shared<FieldDescriptor>(t, name, doc, md);
}

std::shared_ptr<FieldDescriptor> test_message_field(std::string name = "field_name", std::string doc = "field doc string", std::shared_ptr<MessageDescriptor> md = test_message(), std::shared_ptr<MessageDescriptor> type_md = test_message()) {
  return std::make_shared<FieldDescriptor>(name, doc, md, type_md);
}

TEST_CASE("FieldDescriptor.IsScalar", "[FieldDescriptor]") {
  SECTION("IsScalar = true") {
    auto fd_true = test_field(FieldDescriptor::TYPE_STRING);
    CHECK(fd_true->IsScalar());
  }

  SECTION("IsScalar = false") {
    auto fd_false = test_field(FieldDescriptor::TYPE_MESSAGE);
    CHECK_FALSE(fd_false->IsScalar());
  }
}

TEST_CASE("FieldDescriptor.name", "[FieldDescriptor]") {
  const std::string name = "a_field_name";
  auto fd = test_field(FieldDescriptor::TYPE_BOOL, name);
  CHECK(fd->name() == name);
}

TEST_CASE("FieldDescriptor.message", "[FieldDescriptor]") {
  auto md = test_message();
  auto fd = test_field(FieldDescriptor::TYPE_TIME, "created_at", "", md);
  CHECK(fd->message().lock().get() == md.get());
}

TEST_CASE("FieldDescriptor.doc", "[FieldDescriptor]") {
  std::string doc = "This describes the entities name";
  auto fd = test_field(FieldDescriptor::TYPE_STRING, "name", doc);
  CHECK(fd->doc() == doc);
}

TEST_CASE("FieldDescriptor.type", "[FieldDescriptor]") {
  auto fd = test_field(FieldDescriptor::TYPE_INT);
  CHECK(fd->type() == FieldDescriptor::TYPE_INT);
}

TEST_CASE("FieldDescriptor.type_message", "[FieldDescriptor]") {
  SECTION("Scalar field") {
    auto fd = test_field(FieldDescriptor::TYPE_BOOL);
    CHECK_THROWS_AS(fd->type_message(), FieldDescriptor::FieldIsScalarException);
  }
  SECTION("Message field") {
    auto md = test_message();
    auto type_md = test_message();
    auto fd = test_message_field("field_name", "", md, type_md);
    CHECK(fd->type_message().lock().get() == type_md.get());
  }
}


