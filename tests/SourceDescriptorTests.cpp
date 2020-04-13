#include "catch2/catch.hpp"
#include <memory>
#include "../compiler/SourceDescriptor.h"

const char* directive_key = "directive_key";
const char* directive_value = "directive_value";
const char* message_name = "message_name";
const char* message_doc = "This is the docstring";

namespace til {
std::unique_ptr<SourceDescriptor> test_source_descriptor() {
  return std::make_unique<SourceDescriptor>();
}

TEST_CASE("SourceDescriptor.AddDirective") {

  SECTION("Add new directives") {
    auto sd = test_source_descriptor();
    CHECK_NOTHROW(sd->AddDirective(directive_key, directive_value));
    CHECK_NOTHROW(sd->AddDirective("other_directive_key", "other_directive_value"));
  }SECTION("Add existing directive") {
    auto sd = test_source_descriptor();
    CHECK_NOTHROW(sd->AddDirective(directive_key, directive_value));
    CHECK_THROWS_AS(sd->AddDirective(directive_key, directive_value), SourceDescriptor::ItemAlreadyExistsException);
  }
}

TEST_CASE("SourceDescriptor.HasDirective") {
  auto sd = test_source_descriptor();
  sd->AddDirective(directive_key, directive_value);
  SECTION("Has directive") {
    CHECK(sd->HasDirective(directive_key));
  }SECTION("Doesn't have directive") {
    CHECK_FALSE(sd->HasDirective("other"));
  }
}

TEST_CASE("SourceDescriptor.Directive") {
  auto sd = test_source_descriptor();
  sd->AddDirective(directive_key, directive_value);
  SECTION("Has directive") {
    std::string directive = sd->Directive(directive_key);
    CHECK(directive==directive_value);
  }SECTION("Doesn't have directive") {
    CHECK_THROWS_AS(sd->Directive("other"), SourceDescriptor::ItemNotFoundException);
  }
}

TEST_CASE("SourceDescriptor.AddMessage") {
  SECTION("New messages") {
    auto sd = test_source_descriptor();
    CHECK_NOTHROW(sd->AddMessage(message_name, message_doc));
    CHECK_NOTHROW(sd->AddMessage("other_message", "other doc"));
  }SECTION("Existing message") {
    auto sd = test_source_descriptor();
    CHECK_NOTHROW(sd->AddMessage(message_name, message_doc));
    CHECK_THROWS_AS(sd->AddMessage(message_name, message_doc), SourceDescriptor::ItemAlreadyExistsException);
  }
}

TEST_CASE("SourceDescriptor.MessageCount") {
  SECTION("No messages") {
    auto sd = test_source_descriptor();
    CHECK(sd->MessageCount()==0);
  }SECTION("With message") {
    auto sd = test_source_descriptor();
    sd->AddMessage(message_name, message_doc);
    CHECK(sd->MessageCount()==1);
  }
}

TEST_CASE("SourceDescriptor.Message") {
  SECTION("Missing message") {
    auto sd = test_source_descriptor();
    CHECK_THROWS_AS(sd->Message(0), std::out_of_range);
  }SECTION("With message") {
    auto sd = test_source_descriptor();
    auto md = sd->AddMessage(message_name, message_doc);
    CHECK(sd->Message(0).get()==md.get());
  }
}

TEST_CASE("SourceDescriptor.MessageByName") {
  SECTION("Missing message") {
    auto sd = test_source_descriptor();
    CHECK_THROWS_AS(sd->MessageByName(message_name), SourceDescriptor::ItemNotFoundException);
  }SECTION("With message") {
    auto sd = test_source_descriptor();
    auto md = sd->AddMessage(message_name, message_doc);
    CHECK(sd->MessageByName(message_name).get()==md.get());
  }
}

} // namespace til
