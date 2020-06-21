#ifndef TELL_COMPILER_SERIALIZABLECALL_H_
#define TELL_COMPILER_SERIALIZABLECALL_H_

#import <memory>
#import <string>

#import "nlohmann/json.hpp"

using nlohmann::json;

namespace til {
/**
 * Representation of a service call for easy serialization to and from JSON
 */
struct SerializableCall {
  SerializableCall() = default;
  SerializableCall(std::string nm, std::string doc, std::string arg, std::string ret)
      : name(std::move(nm)), doc_comment(std::move(doc)), argument(std::move(arg)), returns(std::move(ret)) {}
  std::string name;
  std::string doc_comment;
  std::string argument;
  std::string returns;
};

void to_json(json &j, const SerializableCall &sc);

void from_json(const json &j, SerializableCall &sc);

}

#endif //TELL_COMPILER_SERIALIZABLECALL_H_
