#include <utility>

//
// Created by ANDREW SMITH on 02/06/2020.
//

#ifndef TELL_COMPILER_SERIALIZABLEMESSAGEDECLARATION_H_
#define TELL_COMPILER_SERIALIZABLEMESSAGEDECLARATION_H_

#import <memory>
#import <string>
#import <vector>

#import "nlohmann/json.hpp"

using nlohmann::json;

#import "SerializableField.h"

namespace til {
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