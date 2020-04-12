//
// Created by ANDREW SMITH on 11/04/2020.
//

#ifndef TELL_COMPILER_SOURCEDESCRIPTOR_H_
#define TELL_COMPILER_SOURCEDESCRIPTOR_H_

#include <map>
#include <memory>
#include <vector>

#include "MessageDescriptor.h"

class SourceDescriptor {
 public:

  class ItemAlreadyExistsException : public std::exception {
   public:
    ItemAlreadyExistsException(std::string name);

    const char *what();
   private:
    std::string name_;
  };

  class ItemNotFoundException : public std::exception {
   public:
    ItemNotFoundException(std::string name);

    const char *what();
   private:
    std::string name_;
  };

  void AddDirective(std::string key, std::string value);
  bool HasDirective(const std::string &key);
  std::string Directive(const std::string &key);

  std::shared_ptr<MessageDescriptor> AddMessage(const std::string &name, const std::string& doc);
  int MessageCount();
  std::shared_ptr<MessageDescriptor> Message(int idx);
  std::shared_ptr<MessageDescriptor> MessageByName(const std::string &name);


 private:
  std::map<std::string, std::string> directives_{}; // always initialize to empty
  std::vector<std::shared_ptr<MessageDescriptor>> messages_;
  std::map<std::string, int> messages_by_name_{}; // always initialize to empty
};

#endif //TELL_COMPILER_SOURCEDESCRIPTOR_H_
