#include "catch2/catch.hpp"
#include <memory>
#include "../compiler/FieldDescriptor.h"
#include "../compiler/MessageDescriptor.h"

namespace til {
std::shared_ptr<MessageDescriptor> test_message() {
  return std::make_shared<MessageDescriptor>("MessageName", "message docstring");
}

std::shared_ptr<FieldDescriptor> test_field(FieldDescriptor::Type t = FieldDescriptor::TYPE_STRING,
                                            bool optional = false,
                                            std::string name = "field_name",
                                            std::string doc = "field doc string",
                                            std::shared_ptr<MessageDescriptor> md = test_message()) {
  return std::make_shared<FieldDescriptor>(t, optional, name, doc, md);
}

TEST_CASE("FieldDescriptor.IsScalar") {
  SECTION("IsScalar = true") {
    auto fd_true = test_field(FieldDescriptor::TYPE_STRING);
    CHECK(fd_true->IsScalar());
  }

  SECTION("IsScalar = false") {
    auto fd_false = test_field(FieldDescriptor::TYPE_MESSAGE);
    CHECK_FALSE(fd_false->IsScalar());
  }
}

TEST_CASE("FieldDescriptor.name") {
  const std::string name = "a_field_name";
  auto fd = test_field(FieldDescriptor::TYPE_BOOL, false, name);
  CHECK(fd->name()==name);
}

TEST_CASE("FieldDescriptor.message") {
  auto md = test_message();
  auto fd = test_field(FieldDescriptor::TYPE_TIME, false, "created_at", "", md);
  CHECK(fd->message().lock().get()==md.get());
}

TEST_CASE("FieldDescriptor.doc") {
  std::string doc = "This describes the entities name";
  auto fd = test_field(FieldDescriptor::TYPE_STRING, false, "name", doc);
  CHECK(fd->doc()==doc);
}

TEST_CASE("FieldDescriptor.type") {
  auto fd = test_field(FieldDescriptor::TYPE_INT);
  CHECK(fd->type()==FieldDescriptor::TYPE_INT);
}

TEST_CASE("FieldDescriptor.SetMessageType") {
  SECTION("Scalar field") {
    auto fd = test_field(FieldDescriptor::TYPE_BOOL);
    CHECK_THROWS_AS(fd->SetTypeMessage(test_message()), FieldDescriptor::FieldIsScalarException);
  }SECTION("Message Field") {
    auto md = test_message();
    auto fd = test_field(FieldDescriptor::TYPE_MESSAGE);
    fd->SetTypeMessage(md);
    CHECK(fd->TypeMessage().lock().get()==md.get());
  }
}

TEST_CASE("FieldDescriptor.TypeMessage") {
  SECTION("Scalar field") {
    auto fd = test_field(FieldDescriptor::TYPE_FLOAT);
    CHECK_THROWS_AS(fd->TypeMessage(), FieldDescriptor::FieldIsScalarException);
  }SECTION("Message field") {
    auto md = test_message();
    auto fd = test_field(FieldDescriptor::TYPE_MESSAGE);
    fd->SetTypeMessage(md);
    CHECK(fd->TypeMessage().lock().get()==md.get());
  }
}

TEST_CASE("FieldDescriptor.optional") {
  SECTION("Required scalar field") {
    CHECK_FALSE(test_field()->optional());
  }SECTION("Optional scalar field") {
    CHECK(test_field(FieldDescriptor::TYPE_BOOL, true)->optional());
  }SECTION("Optional message field") {
    auto md = test_message();
    auto fd = test_field(FieldDescriptor::TYPE_MESSAGE, true);
    fd->SetTypeMessage(md);
    CHECK(fd->optional());
  }
}

} // namespace til


