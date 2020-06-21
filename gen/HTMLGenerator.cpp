#include "HTMLGenerator.h"

#include <memory>
#include <string>

#include "fmt/format.h"
#include "nlohmann/json.hpp"

#include "HTMLTemplate.h"

using nlohmann::json;

std::string gen::HTMLGenerator::Generate(const til::SerializableAST &ast) {
  auto env = PrepareEnvironment();
  json data(ast);
  return env.render(html_template, data);
}

inja::Environment gen::HTMLGenerator::PrepareEnvironment() {
  inja::Environment env;
  env.add_callback("map_type", 1, [this](inja::Arguments &args) {
    auto td = args[0]->get<std::unique_ptr<til::SerializableTypeDef>>();
    return this->MapType(td.get());
  });

  return env;
}

std::string gen::HTMLGenerator::MapType(const til::SerializableTypeDef *td) {
  switch (td->t()) {
    case til::TypeDef::kScalar:
      return this->MapType(dynamic_cast<const til::ScalarSerializableTypeDef *>(td));
    case til::TypeDef::kMessage:
      return this->MapType(dynamic_cast<const til::MessageSerializableTypeDef *>(td));
    case til::TypeDef::kMap:
      return this->MapType(dynamic_cast<const til::MapSerializableTypeDef *>(td));
    case til::TypeDef::kList:
      return this->MapType(dynamic_cast<const til::ListSerializableTypeDef *>(td));
    default:
      throw std::runtime_error("unknown type_def type");
  }
}

std::string gen::HTMLGenerator::MapType(const til::ScalarSerializableTypeDef *td) {
  std::string repr;
  switch (td->scalar_type) {
    case til::ScalarTypeDef::kBool:
      repr = "bool";
      break;
    case til::ScalarTypeDef::kFloat:
      repr = "float";
      break;
    case til::ScalarTypeDef::kInt:
      repr = "int";
      break;
    case til::ScalarTypeDef::kString:
      repr = "string";
      break;
    case til::ScalarTypeDef::kTime:
      repr = "time";
      break;
  }
  return repr;
}

std::string gen::HTMLGenerator::MapType(const til::MessageSerializableTypeDef *td) {
  return td->name;
}

std::string gen::HTMLGenerator::MapType(const til::MapSerializableTypeDef *td) {
  return fmt::format("map[{}]", MapType(td->sub_type.get()));
}

std::string gen::HTMLGenerator::MapType(const til::ListSerializableTypeDef *td) {
  return fmt::format("list[{}]", MapType(td->sub_type.get()));
}


