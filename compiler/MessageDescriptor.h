#ifndef TELL_COMPILER_MESSAGEDESCRIPTOR_H_
#define TELL_COMPILER_MESSAGEDESCRIPTOR_H_
#include <memory>
#include <string>
#include <vector>
#include "FieldDescriptor.h"

class MessageDescriptor: public std::enable_shared_from_this<MessageDescriptor> {
 public:
  MessageDescriptor(std::string name, std::string doc);

  std::string name();

  std::shared_ptr<FieldDescriptor>AddField(FieldDescriptor::Type t, bool optional, const std::string &name, const std::string &doc);
  int FieldCount();
  std::shared_ptr<FieldDescriptor>Field(int idx);

 private:
  std::string name_;
  std::string doc_;
  std::vector<std::shared_ptr<FieldDescriptor>> fields_;
};

#endif //TELL_COMPILER_MESSAGEDESCRIPTOR_H_
