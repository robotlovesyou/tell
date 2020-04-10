//
// Created by ANDREW SMITH on 10/04/2020.
//

#include "FieldDescriptor.h"
#include "fmt/core.h"

FieldDescriptor::FieldDescriptor(FieldDescriptor::Type t, std::string name, std::string doc, const std::shared_ptr<MessageDescriptor> &message): type_(t), name_(std::move(name)), doc_(std::move(doc)), message_(message) {
}

FieldDescriptor::FieldDescriptor(std::string name,
                                 std::string doc,
                                 const std::shared_ptr<MessageDescriptor> &message,
                                 const std::shared_ptr<MessageDescriptor> &type_message): type_(TYPE_MESSAGE), name_(std::move(name)), doc_(std::move(doc)), message_(message), type_message_(std::optional<std::weak_ptr<MessageDescriptor>>(type_message)) {
}

bool FieldDescriptor::IsScalar() {
  return type_ != TYPE_MESSAGE;
}

std::string FieldDescriptor::name() {
  return name_;
}

std::weak_ptr<MessageDescriptor> FieldDescriptor::message() {
  return message_;
}

std::string FieldDescriptor::doc() {
  return doc_;
}

FieldDescriptor::Type FieldDescriptor::type() {
  return type_;
}

std::weak_ptr<MessageDescriptor> FieldDescriptor::type_message() {
  if (IsScalar()) {
    throw FieldIsScalarException(shared_from_this());
  }
  return type_message_.value();
}

const char *FieldDescriptor::FieldIsScalarException::what() {
  return fmt::format("field {} is scalar and has no message_type", fd_->name_).c_str();
}

FieldDescriptor::FieldIsScalarException::FieldIsScalarException(std::shared_ptr<FieldDescriptor> fd): fd_(std::move(fd)) {
}
