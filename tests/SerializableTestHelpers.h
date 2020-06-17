
#ifndef TELL_TESTS_SERIALIZABLETESTHELPERS_H_
#define TELL_TESTS_SERIALIZABLETESTHELPERS_H_

#import <memory>
#import <vector>

#import "SerializableTypeDef.h"
#import "SerializableField.h"
#import "SerializableMessageDeclaration.h"
#import "SerializableServiceDeclaration.h"

static std::unique_ptr<til::SerializableTypeDef> test_scalar_serializable_type_def(til::ScalarTypeDef::ScalarType scalar_type = til::ScalarTypeDef::kString, bool optional = true) {
  return std::make_unique<til::ScalarSerializableTypeDef>(scalar_type, optional);
}

static std::unique_ptr<til::SerializableField> test_serializable_field(const std::string& name, const std::string& doc, std::unique_ptr<til::SerializableTypeDef> type_def) {
  return std::make_unique<til::SerializableField>(name, doc, std::move(type_def));
}

static std::unique_ptr<til::SerializableMessageDeclaration> test_serializable_message_declaration(const std::string& name = "AMessageName", const std::string& doc = "Some docs") {
  std::vector<std::unique_ptr<til::SerializableField>> fields;
  fields.push_back(std::move(test_serializable_field("field_a", "doc", std::move(test_scalar_serializable_type_def()))));
  fields.push_back(std::move(test_serializable_field("field_b", "doc 2", std::move(test_scalar_serializable_type_def()))));
  return std::make_unique<til::SerializableMessageDeclaration>(name, doc, std::move(fields));
}

static std::unique_ptr<til::SerializableServiceDeclaration> test_serializable_service_declaration(const std::string &name = "AService", const std::string &doc = "Some docs") {
  std::vector<std::unique_ptr<til::SerializableCall>> calls;
  calls.push_back(std::move(std::make_unique<til::SerializableCall>("call_a", "docs", "AnArgument", "AReturnValue")));
  calls.push_back(std::move(std::make_unique<til::SerializableCall>("call_b", "docs", "AnotherArgument", "AnotherReturnValue")));
  return std::make_unique<til::SerializableServiceDeclaration>(name, doc, std::move(calls));
}

#endif //TELL_TESTS_SERIALIZABLETESTHELPERS_H_
