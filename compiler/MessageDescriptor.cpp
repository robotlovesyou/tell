//
// Created by ANDREW SMITH on 10/04/2020.
//

#include "MessageDescriptor.h"

MessageDescriptor::MessageDescriptor(std::string name, std::string doc): name_(std::move(name)), doc_(std::move(doc)) {
}

std::string MessageDescriptor::name() {
  return name_;
}

std::shared_ptr<FieldDescriptor> MessageDescriptor::AddField(FieldDescriptor::Type t,
                                                             bool optional,
                                                             const std::string &name,
                                                             const std::string &doc) {
  auto fd = std::make_shared<FieldDescriptor>(t, optional, name, doc, shared_from_this());
  fields_.push_back(fd);
  return fd;
}

int MessageDescriptor::FieldCount() {
  return fields_.size();
}
std::shared_ptr<FieldDescriptor> MessageDescriptor::Field(int idx) {
  return fields_.at(idx);
}

