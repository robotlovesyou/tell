#ifndef TELL_COMPILER_MESSAGEDESCRIPTOR_H_
#define TELL_COMPILER_MESSAGEDESCRIPTOR_H_
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "FieldDescriptor.h"

/**
 * Describes a message.
 */
class MessageDescriptor : public std::enable_shared_from_this<MessageDescriptor> {
 public:

  /**
   * FieldAlreadyExistsException is thrown when adding a field with a duplicate name
   */
  class FieldAlreadyExistsException : public std::exception {
   public:
    /**
     * Constructor
     * @param message The message which is the subject of the exception
     * @param name The name of the clashing field
     */
    FieldAlreadyExistsException(std::shared_ptr<MessageDescriptor> message,
                                std::string name);

    /**
     * Describes the error
     * @return The description
     */
    const char *what();
   private:
    std::shared_ptr<MessageDescriptor> message_;
    std::string name_;
  };
  /**
   * Construct a MessageDescriptor
   * @param name The name of the message
   * @param doc The docstring for the message
   */
  MessageDescriptor(std::string name, std::string doc);

  /**
   * Accessor for the name of the message
   * @return
   */
  std::string name();

  /**
   * Factory function to add a Field to the message
   * @param t The type of the field
   * @param optional Whether the field is optional
   * @param name The name of the field
   * @param doc The docstring for the field
   * @return A shared_ptr to the new field
   */
  std::shared_ptr<FieldDescriptor> AddField(FieldDescriptor::Type t,
                                            bool optional,
                                            const std::string &name,
                                            const std::string &doc);

  /**
   * Return the current field count
   * @return
   */
  int FieldCount();

  /**
   * Return a shared_ptr to the field at index idx. Throws std::out_of_range if idx exceeds FieldCount-1.
   * @param idx The index of the field to return
   * @return A shared_ptr to the field at index idx
   */
  std::shared_ptr<FieldDescriptor> Field(int idx);

 private:
  std::string name_;
  std::string doc_;
  std::vector<std::shared_ptr<FieldDescriptor>> fields_;
  std::map<std::string, int> fields_by_name_{}; // initialize here. not set up by constructor
};

#endif //TELL_COMPILER_MESSAGEDESCRIPTOR_H_
