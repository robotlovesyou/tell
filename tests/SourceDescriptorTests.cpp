#include "catch2/catch.hpp"
#include <memory>
#include "../compiler/SourceDescriptor.h"

TEST_CASE("SourceDescriptor.AddDirective") {
  std::string key = "directive_key";
  std::string value = "directive_value";
  SECTION("Add new directive") {
    SourceDescriptor sd;
    CHECK_NOTHROW(sd.AddDirective(key, value));
  }
  SECTION("Add existing directive") {
    auto sd = std::make_unique<SourceDescriptor>();
    CHECK_NOTHROW(sd->AddDirective(key, value));
    CHECK_THROWS_AS(sd->AddDirective(key, value), SourceDescriptor::ItemAlreadyExistsException);
  }
}

TEST_CASE("SourceDescriptor.HasDirective") {
  std::string key = "directive_key";
  std::string value = "directive_value";
  auto sd = std::make_unique<SourceDescriptor>();
  sd->AddDirective(key, value);
  SECTION("Has directive") {
    CHECK(sd->HasDirective(key));
  }
  SECTION("Doesn't have directive") {
    CHECK_FALSE(sd->HasDirective("other"));
  }
}

TEST_CASE("SourceDescriptor.Directive") {
  std::string key = "directive_key";
  std::string value = "directive_value";
  auto sd = std::make_unique<SourceDescriptor>();
  sd->AddDirective(key, value);
  SECTION("Has directive") {
    std::string directive = sd->Directive(key);
    CHECK(directive == value);
  }
  SECTION("Doesn't have directive") {
    CHECK_THROWS_AS(sd->Directive("other"), SourceDescriptor::ItemNotFoundException);
  }
}

TEST_CASE("SourceDescriptor.AddMessage", "[SourceDescriptor]") {
  FAIL("pending");
}

TEST_CASE("SourceDescriptor.MessageCount", "[SourceDescriptor]") {
  FAIL("pending");
}

TEST_CASE("SourceDescriptor.Message", "[SourceDescriptor]") {
  FAIL("pending");
}

TEST_CASE("SourceDescriptor.MessageByName", "[SourceDescriptor]") {
  FAIL("pending");
}

