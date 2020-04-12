//
// Created by ANDREW SMITH on 10/04/2020.
//

#include "FieldDescriptor.h"
#include "fmt/core.h"

namespace til {
FieldDescriptor::FieldDescriptor(FieldDescriptor::Type t,
                                 bool optional,
                                 std::string name,
                                 std::string doc,
                                 const std::shared_ptr<MessageDescriptor> &message)
    : type_(t), optional_(optional), name_(std::move(name)), doc_(std::move(doc)), message_(message) {
}

bool FieldDescriptor::IsScalar() {
  return type_!=TYPE_MESSAGE;
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

std::weak_ptr<MessageDescriptor> FieldDescriptor::TypeMessage() {
  if (IsScalar()) {
    throw FieldIsScalarException(shared_from_this());
  }
  return type_message_.value();
}

bool FieldDescriptor::optional() {
  return optional_;
}

void FieldDescriptor::SetTypeMessage(const std::shared_ptr<MessageDescriptor> &message) {
  if (type_!=TYPE_MESSAGE) {
    throw FieldIsScalarException(shared_from_this());
  }
  type_message_ = std::optional<std::weak_ptr<MessageDescriptor>>(message);
}

const char *FieldDescriptor::FieldIsScalarException::what() {
  return fmt::format("field {} is scalar and has no message_type", fd_->name_).c_str();
}

FieldDescriptor::FieldIsScalarException::FieldIsScalarException(std::shared_ptr<FieldDescriptor> fd)
    : fd_(std::move(fd)) {
}

} // namespace til

