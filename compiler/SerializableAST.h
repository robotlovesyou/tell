#ifndef TELL_COMPILER_SERIALIZABLEAST_H_
#define TELL_COMPILER_SERIALIZABLEAST_H_

#import <memory>
#import <string>
#import <vector>

#import "nlohmann/json.hpp"
using nlohmann::json;

#include "SerializableMessageDeclaration.h"
#include "SerializableDirectiveDeclaration.h"

namespace til {
struct SerializableAST {
  SerializableAST() = default;
  explicit SerializableAST(std::vector<std::unique_ptr<SerializableDirectiveDeclaration>> dir_decls,
                           std::vector<std::unique_ptr<SerializableMessageDeclaration>> msg_decls)
      : directive_declarations(std::move(dir_decls)), message_declarations(std::move(msg_decls)) {}

  std::vector<std::unique_ptr<SerializableDirectiveDeclaration>> directive_declarations;
  std::vector<std::unique_ptr<SerializableMessageDeclaration>> message_declarations;
};

void to_json(json &j, const SerializableAST &ast);
void from_json(const json &j, SerializableAST &ast);
}

#endif //TELL_COMPILER_SERIALIZABLEAST_H_
