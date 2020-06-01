//
// Created by ANDREW SMITH on 01/06/2020.
//

#ifndef TELL_COMPILER_SERIALIZABLEFIELD_H_
#define TELL_COMPILER_SERIALIZABLEFIELD_H_

#include <memory>
#include <string>
#include <utility>

#include "nlohmann/json.hpp"

#include "Field.h"
#include "TypeDef.h"

using nlohmann::json;

namespace til {
class SerializableField {
  SerializableField() = default;
  SerializableField(std::string nm, std::string doc, std::unique_ptr<TypeDef> td)
      : name(std::move(nm)), doc_comment(std::move(doc)), type_def(std::move(td)) {}
  std::string name{};
  std::string doc_comment{};
  std::unique_ptr<TypeDef> type_def{};
};

void to_json(json &j, const SerializableField &field);

void from_json(const json &j, SerializableField &field);
}

#endif //TELL_COMPILER_SERIALIZABLEFIELD_H_
