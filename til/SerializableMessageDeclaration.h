#ifndef TELL_COMPILER_SERIALIZABLEMESSAGEDECLARATION_H_
#define TELL_COMPILER_SERIALIZABLEMESSAGEDECLARATION_H_

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "nlohmann/json.hpp"

using nlohmann::json;

#include "SerializableField.h"

namespace til {
/**
 * Representation of a message declaration for easy serialization to and from JSON
 */
struct SerializableMessageDeclaration {
  SerializableMessageDeclaration() = default;
  SerializableMessageDeclaration(std::string nm, std::string doc, std::vector<std::unique_ptr<SerializableField>> flds)
      : name(std::move(nm)), doc_comment(std::move(doc)), fields(std::move(flds)) {}
  std::string name;
  std::string doc_comment;
  std::vector<std::unique_ptr<SerializableField>> fields;
};

void to_json(json &j, const SerializableMessageDeclaration &smd);

void from_json(const json &j, SerializableMessageDeclaration &smd);

}

#endif //TELL_COMPILER_SERIALIZABLEMESSAGEDECLARATION_H_
