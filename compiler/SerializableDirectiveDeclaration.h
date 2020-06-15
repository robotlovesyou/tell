#ifndef TELL_COMPILER_SERIALIZABLEDIRECTIVEDECLARATION_H_
#define TELL_COMPILER_SERIALIZABLEDIRECTIVEDECLARATION_H_

#include <memory>
#include <string>

#include "nlohmann/json.hpp"

using nlohmann::json;

namespace til {
struct SerializableDirectiveDeclaration {
  SerializableDirectiveDeclaration() = default;
  SerializableDirectiveDeclaration(std::string nm, std::string val): name(std::move(nm)), value(std::move(val)) {}
  std::string name;
  std::string value;
};

void to_json(json &j, const SerializableDirectiveDeclaration &sdd);
void from_json(const json &j, SerializableDirectiveDeclaration &sdd);
}

#endif //TELL_COMPILER_SERIALIZABLEDIRECTIVEDECLARATION_H_
