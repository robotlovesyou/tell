#ifndef TELL_COMPILER_SERIALIZABLESERVICEDECLARATION_H_
#define TELL_COMPILER_SERIALIZABLESERVICEDECLARATION_H_

#import <memory>
#import <string>

#import "nlohmann/json.hpp"

#import "SerializableCall.h"

namespace til {
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
