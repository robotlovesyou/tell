//
// Created by ANDREW SMITH on 10/04/2020.
//

#ifndef TELL_FIELDDESCRIPTOR_H
#define TELL_FIELDDESCRIPTOR_H

#include <exception>
#include <memory>
#include <optional>
#include <string>

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
    TYPE_MESSAGE = 6
  };

  /**
   * FieldIsScalarException is thrown if any field method requiring the field to be a message type is accessed
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
   * Scalar field constructor
   * @param t Type of the field.
   * @param optional Whether the field is optional
   * @param name Name of the field
   * @param doc Docstring for the field
   * @param message Containing message
   */
  FieldDescriptor(Type t, bool optional, std::string name, std::string doc, const std::shared_ptr<MessageDescriptor> &message);

  /**
   * Is this a scalar or message type field?
   * @return false if the type is TYPE_MESSAGE, otherwise true
   */
  bool IsScalar();

  /**
   * If the field is a TYPE_MESSAGE then returns the MessageDescriptor for the field.
   * If the field is a scalar type throws a FieldIsScalar exception.
   * @return
   */
  std::weak_ptr<MessageDescriptor> TypeMessage();

  /**
   * Set the MessageType of this field.
   * If the field is not a TYPE_MESSAGE it throws a FieldIsScalar exception.
   * @param message
   */
  void SetTypeMessage(const std::shared_ptr<MessageDescriptor> &message);

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
  bool optional ();

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
  Type type_;
  std::string name_;
  std::string doc_;
  bool optional_;
  std::weak_ptr<MessageDescriptor> message_;
  std::optional<std::weak_ptr<MessageDescriptor>> type_message_;

};

#endif // TELL_FIELDDESCRIPTOR_H
