#ifndef TELL_COMPILER_SERIALIZABLESERVICEDECLARATION_H_
#define TELL_COMPILER_SERIALIZABLESERVICEDECLARATION_H_

#include <memory>
#include <string>

#include "nlohmann/json.hpp"

#include "SerializableCall.h"

namespace til {
/**
 * Representation of a service declaration for easy serialization to and from JSON
 */
struct SerializableServiceDeclaration {
  SerializableServiceDeclaration() = default;
  SerializableServiceDeclaration(std::string nm, std::string doc, std::vector<std::unique_ptr<SerializableCall>> cls)
      : name(std::move(nm)), doc_comment(std::move(doc)), calls(std::move(cls)) {}
  std::string name;
  std::string doc_comment;
  std::vector<std::unique_ptr<SerializableCall>> calls;
};

void to_json(json &j, const SerializableServiceDeclaration &sd);
void from_json(const json &j, SerializableServiceDeclaration &sd);
}

#endif //TELL_COMPILER_SERIALIZABLESERVICEDECLARATION_H_
