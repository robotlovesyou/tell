//
// Created by ANDREW SMITH on 10/04/2020.
//

#include "MessageDescriptor.h"
#include "fmt/core.h"

MessageDescriptor::MessageDescriptor(std::string name, std::string doc) : name_(std::move(name)), doc_(std::move(doc)) {
}

std::string MessageDescriptor::name() {
  return name_;
}

std::shared_ptr<FieldDescriptor> MessageDescriptor::AddField(FieldDescriptor::Type t,
                                                             bool optional,
                                                             const std::string &name,
                                                             const std::string &doc) {
  if (fields_by_name_.count(name)!=0) {
    throw FieldAlreadyExistsException(shared_from_this(), name);
  }
  auto fd = std::make_shared<FieldDescriptor>(t, optional, name, doc, shared_from_this());
  fields_.push_back(fd);
  fields_by_name_[name] = fields_.size() - 1;
  return fd;
}

int MessageDescriptor::FieldCount() {
  return fields_.size();
}

std::shared_ptr<FieldDescriptor> MessageDescriptor::Field(int idx) {
  return fields_.at(idx);
}

MessageDescriptor::FieldAlreadyExistsException::FieldAlreadyExistsException(std::shared_ptr<MessageDescriptor> message,
                                                                            std::string name) : message_(std::move(
    message)), name_(std::move(name)) {
}

const char *MessageDescriptor::FieldAlreadyExistsException::what() {
  return fmt::format("message {} already contains a field named {}.", message_->name(), name_).c_str();
}
