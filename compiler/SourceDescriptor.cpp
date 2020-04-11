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
  if (directives_.count(key) != 0) {
    throw ItemAlreadyExistsException(key);
  }
  directives_[std::move(key)] = std::move(value);
}

bool SourceDescriptor::HasDirective(const std::string &key) {
  return directives_.count(key) == 1;
}

std::string SourceDescriptor::Directive(const std::string &key) {
  if (directives_.count(key) != 1) {
    throw ItemNotFoundException(key);
  }
  return directives_[key];
}

std::shared_ptr<MessageDescriptor> SourceDescriptor::AddMessage(std::string name, std::string doc) {
  return std::shared_ptr<MessageDescriptor>();
}

int SourceDescriptor::MessageCount() {
  return 0;
}

std::shared_ptr<MessageDescriptor> SourceDescriptor::Message(int) {
  return std::shared_ptr<MessageDescriptor>();
}

std::shared_ptr<MessageDescriptor> SourceDescriptor::MessageByName(const std::string &name) {
  return std::shared_ptr<MessageDescriptor>();
}

SourceDescriptor::ItemNotFoundException::ItemNotFoundException(std::string name) {

}

const char *SourceDescriptor::ItemNotFoundException::what() {
  return nullptr;
}
