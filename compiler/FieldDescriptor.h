//
// Created by ANDREW SMITH on 10/04/2020.
//

#ifndef TELL_FIELDDESCRIPTOR_H
#define TELL_FIELDDESCRIPTOR_H

#include <exception>
#include <memory>
#include <optional>
#include <string>

class MessageDescriptor;

class FieldDescriptor : public std::enable_shared_from_this<FieldDescriptor> {
 public:
  /**
   * Type is used to describe the type of the field.
   */
  enum Type {
    TYPE_FLOAT = 1,
    TYPE_INT = 2,
    TYPE_BOOL = 3,
    TYPE_STRING = 4,
    TYPE_TIME = 5,
    TYPE_MESSAGE = 6
  };

  /**
   * FieldIsScalarException is thrown if any field method requiring the field to be a message type is accessed
   */
  class FieldIsScalarException : public std::exception {
   public:
    explicit FieldIsScalarException(std::shared_ptr<FieldDescriptor> fd);
    const char *what();
   private:
    std::shared_ptr<FieldDescriptor> fd_;
  };


  FieldDescriptor(Type t, std::string name, std::string doc, const std::shared_ptr<MessageDescriptor> &message);
  FieldDescriptor(std::string name,
                  std::string doc,
                  const std::shared_ptr<MessageDescriptor> &message,
                  const std::shared_ptr<MessageDescriptor> &type_message);
  bool IsScalar();
  std::string name();
  std::string doc();
  std::weak_ptr<MessageDescriptor> message();
  Type type();
  std::weak_ptr<MessageDescriptor> TypeMessage();

 private:
  Type type_;
  std::string name_;
  std::string doc_;
  std::weak_ptr<MessageDescriptor> message_;
  std::optional<std::weak_ptr<MessageDescriptor>> type_message_;

};

#endif // TELL_FIELDDESCRIPTOR_H
