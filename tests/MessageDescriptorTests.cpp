#include "catch2/catch.hpp"
#include "../compiler/MessageDescriptor.h"

namespace til {

std::shared_ptr<MessageDescriptor> test_message(std::string name = "message",
                                                std::string doc = "this is the docstring") {
  return std::make_shared<MessageDescriptor>(name, doc);
}

std::shared_ptr<FieldDescriptor> add_test_field(const std::shared_ptr<MessageDescriptor> &md = test_message(),
                                                FieldDescriptor::Type t = FieldDescriptor::TYPE_STRING,
                                                bool optional = false,
                                                std::string name = "field_name",
                                                std::string doc = "field doc string") {
  return md->AddField(t, optional, name, doc);
}

TEST_CASE("MessageDescriptor.name", "[MessageDescriptor]") {
  std::string name = "MyMessage";
  auto md = test_message(name);
  CHECK(md->name()==name);
}

TEST_CASE("MessageDescriptor.FieldCount", "[MessageDescriptor]") {
  SECTION("No fields") {
    auto md = test_message();
    CHECK(md->FieldCount()==0);
  }SECTION("With fields") {
    auto md = test_message();
    auto fd = add_test_field(md);
    CHECK(md->FieldCount()==1);
  }
}

TEST_CASE("MessageDescriptor.AddField", "[MessageDescriptor]") {
  SECTION("Adds to fields") {
    auto md = test_message();
    CHECK(md->FieldCount()==0);

    auto fd = add_test_field(md);
    CHECK(md->FieldCount()==1);
    auto fd2 = add_test_field(md, FieldDescriptor::TYPE_BOOL, false, "other_name");
    CHECK(md->FieldCount()==2);
  }SECTION("Sets parent refs") {
    auto md = test_message();
    auto fd = add_test_field(md);
    CHECK(fd->message().lock().get()==md.get());
  }SECTION("Throws on clashing name") {
    auto md = test_message();
    add_test_field(md, FieldDescriptor::TYPE_BOOL, false, "clashes");
    CHECK_THROWS_AS(add_test_field(md, FieldDescriptor::TYPE_BOOL, false, "clashes"),
                    MessageDescriptor::FieldAlreadyExistsException);
  }
}

TEST_CASE("MessageDescriptor.Field", "[MessageDescriptor]") {
  SECTION("Valid index") {
    auto md = test_message();
    auto fd = add_test_field(md);
    CHECK(md->Field(0).get()==fd.get());
  }SECTION("Invalid index") {
    auto md = test_message();
    CHECK_THROWS_AS(md->Field(0), std::out_of_range);
  }
}

} // namespace til