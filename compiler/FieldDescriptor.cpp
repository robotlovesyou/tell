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
  return *type_message_;
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

FieldDescriptor::Type FieldDescriptor::SubType() {
  throw_on_no_subtype();
  return sub_type_;
}

std::weak_ptr<MessageDescriptor> FieldDescriptor::SubTypeMessage() {
  throw_on_no_subtype();
  if(sub_type_ != TYPE_MESSAGE) {
    throw FieldIsScalarException(shared_from_this());
  }
  return *sub_type_message_;
}

void FieldDescriptor::SetSubType(const FieldDescriptor::Type &sub_type) {
  throw_on_no_subtype();
  if(sub_type == TYPE_LIST || sub_type == TYPE_MAP) {
    throw IllegalSubtypeException();
  }
  sub_type_ = sub_type;
}

void FieldDescriptor::SetSubTypeMessage(const std::shared_ptr<MessageDescriptor> &message) {
  throw_on_no_subtype();
  if(sub_type_ != TYPE_MESSAGE) {
    throw FieldIsScalarException(shared_from_this());
  }
  sub_type_message_ = std::optional<std::weak_ptr<MessageDescriptor>>(message);
}

void FieldDescriptor::throw_on_no_subtype() {
  if(type_ != TYPE_LIST && type_ != TYPE_MAP) {
    throw FieldHasNoSubtypeException(shared_from_this());
  }
}

const char *FieldDescriptor::FieldIsScalarException::what() {
  return fmt::format("field or subtype {} is scalar and has no message_type", fd_->name_).c_str();
}

FieldDescriptor::FieldIsScalarException::FieldIsScalarException(std::shared_ptr<FieldDescriptor> fd)
    : fd_(std::move(fd)) {
}

FieldDescriptor::FieldHasNoSubtypeException::FieldHasNoSubtypeException(std::shared_ptr<FieldDescriptor> fd): fd_(std::move(fd)) {
}

const char *FieldDescriptor::FieldHasNoSubtypeException::what() {
  return fmt::format("field {} has no subtype", fd_->name_).c_str();
}



const char *FieldDescriptor::IllegalSubtypeException::what() {
  return "cannot set TYPE_MAP or TYPE_LIST as field subtype";
}
} // namespace til

