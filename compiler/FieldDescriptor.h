//
// Created by ANDREW SMITH on 10/04/2020.
//

#ifndef TELL_FIELDDESCRIPTOR_H
#define TELL_FIELDDESCRIPTOR_H

#include <exception>
#include <memory>
#include <optional>
#include <string>

namespace til {
/**
 * Forward declaration of MessageDescriptor. See MessageDescriptor.h/MessageDescriptor.cpp for implementation.
 */
class MessageDescriptor;

/**
 * FieldDescriptor describes a single field of a MessageDescriptor
 */
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
    TYPE_MESSAGE = 6,
    TYPE_LIST = 7,
    TYPE_MAP = 8
  };

  /**
   * FieldIsScalarException is thrown if any field method requiring the field to be a message type is accessed illegally
   */
  class FieldIsScalarException : public std::exception {
   public:
    /**
     * Constructor
     * @param fd The field throwing the exception
     */
    explicit FieldIsScalarException(std::shared_ptr<FieldDescriptor> fd);
    const char *what();
   private:
    std::shared_ptr<FieldDescriptor> fd_;
  };

  /**
   * FieldHasNoSubtypeException is thrown if any field method requiring the field to be have a subtye is accessed illegally
   */
  class FieldHasNoSubtypeException : public std::exception {
   public:
    /**
     * Constructor
     * @param fd The field throwing the exception
     */
    explicit FieldHasNoSubtypeException(std::shared_ptr<FieldDescriptor> fd);
    const char *what();
   private:
    std::shared_ptr<FieldDescriptor> fd_;
  };

  /**
  * IllegalSubtypeException is thrown trying to set the subtype of a message to TYPE_LIST or TYPE_MAP
  */
  class IllegalSubtypeException : public std::exception {
   public:
    const char *what();
   private:
  };

  /**
   * Scalar field constructor
   * @param t Type of the field.
   * @param optional Whether the field is optional
   * @param name Name of the field
   * @param doc Docstring for the field
   * @param message Containing message
   */
  FieldDescriptor(Type t,
                  bool optional,
                  std::string name,
                  std::string doc,
                  const std::shared_ptr<MessageDescriptor> &message);

  /**
   * Is this a scalar or message type field?
   * @return false if the type is TYPE_MESSAGE, TYPE_LIST or TYPE_MAP, otherwise true
   */
  bool IsScalar();

  /**
   * If the field is a TYPE_MESSAGE then returns the MessageDescriptor for the field.
   * If the field is a scalar type throws a FieldIsScalar exception.
   * @return
   */
  std::weak_ptr<MessageDescriptor> TypeMessage();

  /**
   * If the field is a TYPE_LIST or a TYPE_MAP then returns the Type for the sub type.
   * If the field is not a TYPE_LIST or TYPE_MAP then throws a FieldHasNoSubtype exception
   * @return
   */
  Type SubType();

  /**
   * If the field is a TYPE_LIST ora TYPE_MAP and the sub type is a TYPE_MESSAGE then returns the TypeMessage for the
   * sub type.
   * If the field is not a TYPE_LIST or TYPE_MAP then throws a FieldHasNoSubtype exception.
   * If the field subtype is not a TYPE_MESSAGE then throws a FieldIsScalar exception
   * @return
   */
  std::weak_ptr<MessageDescriptor> SubTypeMessage();

  /**
   * Set the MessageType of this field.
   * If the field is not a TYPE_MESSAGE it throws a FieldIsScalar exception.
   * @param message
   */
  void SetTypeMessage(const std::shared_ptr<MessageDescriptor> &message);

  /**
   * Set the sub_type of this field.
   * If the field is not a TYPE_MAP or TYPE_LIST then it throws a FieldHasNoSubtype exception
   * @param sub_type
   */
  void SetSubType(const Type &sub_type);

  /**
   * Set the sub type message of this field.
   * If the field is not a TYPE_MAP or TYPE_LIST then it throws a FieldHasNoSubtype exception
   * If the sub type is not a TYPE_MESSAGE then it throws a field is scalar exception
   * @param message
   */
  void SetSubTypeMessage(const std::shared_ptr<MessageDescriptor> &message);

  /**
   * Accessor for the name property
   * @return
   */
  std::string name();

  /**
   * Accessor for the doc property
   * @return
   */
  std::string doc();

  /**
   * Accessor for the optional property
   * @return
   */
  bool optional();

  /**
   * Accessor for the message property
   * @return
   */
  std::weak_ptr<MessageDescriptor> message();

  /**
   * Accessor for the type property
   * @return
   */
  Type type();

 private:
  void throw_on_no_subtype();
  Type type_;
  Type sub_type_{};
  std::string name_;
  std::string doc_;
  bool optional_;
  std::weak_ptr<MessageDescriptor> message_;
  std::optional<std::weak_ptr<MessageDescriptor>> type_message_{};
  std::optional<std::weak_ptr<MessageDescriptor>> sub_type_message_{};

};
}

#endif // TELL_FIELDDESCRIPTOR_H
