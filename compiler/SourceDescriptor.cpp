//
// Created by ANDREW SMITH on 11/04/2020.
//

#include "SourceDescriptor.h"
#include "fmt/core.h"

SourceDescriptor::ItemAlreadyExistsException::ItemAlreadyExistsException(std::string name) {
  name_ = std::move(name);
}

const char *SourceDescriptor::ItemAlreadyExistsException::what() {
  return fmt::format("Item {} already exists", name_).c_str();
}

void SourceDescriptor::AddDirective(std::string key, std::string value) {
  if (directives_.count(key)!=0) {
    throw ItemAlreadyExistsException(key);
  }
  directives_[std::move(key)] = std::move(value);
}

bool SourceDescriptor::HasDirective(const std::string &key) {
  return directives_.count(key)==1;
}

std::string SourceDescriptor::Directive(const std::string &key) {
  if (directives_.count(key)!=1) {
    throw ItemNotFoundException(key);
  }
  return directives_[key];
}

std::shared_ptr<MessageDescriptor> SourceDescriptor::AddMessage(const std::string &name, const std::string &doc) {
  if (messages_by_name_.count(name)!=0) {
    throw ItemAlreadyExistsException(name);
  }
  auto sd = std::make_shared<MessageDescriptor>(name, doc);
  messages_.push_back(sd);
  messages_by_name_[name] = messages_.size() - 1;
  return sd;
}

int SourceDescriptor::MessageCount() {
  return messages_.size();
}

std::shared_ptr<MessageDescriptor> SourceDescriptor::Message(int idx) {
  return messages_.at(idx);
}

std::shared_ptr<MessageDescriptor> SourceDescriptor::MessageByName(const std::string &name) {
  if (messages_by_name_.count(name)==0) {
    throw ItemNotFoundException(name);
  }
  return messages_[messages_by_name_[name]];
}

SourceDescriptor::ItemNotFoundException::ItemNotFoundException(std::string name) : name_(std::move(name)) {
}

const char *SourceDescriptor::ItemNotFoundException::what() {
  return fmt::format("Item {} not found", name_).c_str();
}
