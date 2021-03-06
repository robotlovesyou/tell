#ifndef TELL_COMPILER_SERIALIZABLEAST_H_
#define TELL_COMPILER_SERIALIZABLEAST_H_

#include <memory>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"
using nlohmann::json;

#include "SerializableDirectiveDeclaration.h"
#include "SerializableMessageDeclaration.h"
#include "SerializableServiceDeclaration.h"

namespace til {
/**
 * The serializbleAST structure is used to serialize the AST
 * to and from a JSON representation.
 * The JSON representation is used both as an output format by the tilc compiler
 * and as a data format by the template library (inja) used by the code generators
 */
struct SerializableAST {
  SerializableAST() = default;
  explicit SerializableAST(std::vector<std::unique_ptr<SerializableDirectiveDeclaration>> dir_decls,
                           std::vector<std::unique_ptr<SerializableMessageDeclaration>> msg_decls,
                           std::vector<std::unique_ptr<SerializableServiceDeclaration>> svc_decls)
      : directive_declarations(std::move(dir_decls)),
        message_declarations(std::move(msg_decls)),
        service_declarations(std::move(svc_decls)) {}

  std::vector<std::unique_ptr<SerializableDirectiveDeclaration>> directive_declarations;
  std::vector<std::unique_ptr<SerializableMessageDeclaration>> message_declarations;
  std::vector<std::unique_ptr<SerializableServiceDeclaration>> service_declarations;
};

void to_json(json &j, const SerializableAST &ast);
void from_json(const json &j, SerializableAST &ast);
}

#endif //TELL_COMPILER_SERIALIZABLEAST_H_
