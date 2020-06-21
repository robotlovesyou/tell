#include "GoGenerator.h"

#include "nlohmann/json.hpp"
#include "fmt/format.h"

#include "GoTemplate.h"
#include "MissingDirectiveException.h"
#include "Strcase.h"

using nlohmann::json;

static const char* GO_PATH = "go_path";
static const char* GO_PACKAGE = "go_package";

std::string gen::GoGenerator::Generate(const til::SerializableAST &ast) {
  // ensure that the ast has a go_package directive
  EnsureDirective(ast, GO_PACKAGE);
  auto env = PrepareEnvironment(ast);
  json data(ast);
  return env.render(go_template, data);
}

void gen::GoGenerator::EnsureDirective(const til::SerializableAST &ast, const std::string& name) {
  for (const std::unique_ptr<til::SerializableDirectiveDeclaration> & sdd: ast.directive_declarations) {
    if(sdd->name == name) {
      return;
    }
  }
  throw MissingDirectiveException(name);
}

inja::Environment gen::GoGenerator::PrepareEnvironment(const til::SerializableAST &ast) {
  inja::Environment env;

  env.add_callback("go_package", 1, [this](inja::Arguments &args) {
    return this->GoPackageName(args);
  });

  env.add_callback("camel_case", 1, [](inja::Arguments &args) {
    auto name = args[0]->get<std::string>();
    return ToCamel(name);
  });

  env.add_callback("lower_camel_case", 1, [](inja::Arguments &args) {
    auto name = args[0]->get<std::string>();
    return ToCamel(name, false);
  });

  env.add_callback("snake_case", 1, [](inja::Arguments &args) {
    auto name = args[0]->get<std::string>();
    return ToSnake(name);
  });

  env.add_callback("has_content", 1, [](inja::Arguments &args) {
    return !args[0]->get<std::string>().empty();
  });

  env.add_callback("map_type", 1, [this](inja::Arguments &args) {
    auto type_def = args[0]->get<std::unique_ptr<til::SerializableTypeDef>>();
    return this->MapType(type_def.get());
  });

  env.set_trim_blocks(true);
  env.set_lstrip_blocks(true);
  return env;
}

std::string gen::GoGenerator::GoPackageName(inja::Arguments &args) {
  const auto *directives = args[0];
  for (const json &element : *directives) {
    auto directive = element.get<til::SerializableDirectiveDeclaration>();
    if (directive.name == GO_PACKAGE) {
      auto package_name = directive.value;
      if(auto pos = package_name.find_last_of('/') != std::string::npos) {
        package_name.erase(0, pos + 1);
      }
      return package_name;
    }
  }
  return "";
}
std::string gen::GoGenerator::MapType(const til::SerializableTypeDef *td) {
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

std::string gen::GoGenerator::MapType(const til::ScalarSerializableTypeDef *td) {
  std::string go_scalar;
  switch (td->scalar_type) {

    case til::ScalarTypeDef::kBool:
      go_scalar = "bool";
      break;
    case til::ScalarTypeDef::kFloat:
      go_scalar = "float64";
      break;
    case til::ScalarTypeDef::kInt:
      go_scalar = "int64";
      break;
    case til::ScalarTypeDef::kString:
      go_scalar = "string";
      break;
    case til::ScalarTypeDef::kTime:
      go_scalar = "time.Time";
      break;
    default:
      throw std::runtime_error("unknown scalar type");
  }
  return (td->is_optional ? "*" : "") + go_scalar;
}
std::string gen::GoGenerator::MapType(const til::MessageSerializableTypeDef *td) {
  return (td->is_optional ? "*" : "") + td->name;
}
std::string gen::GoGenerator::MapType(const til::MapSerializableTypeDef *td) {
  return fmt::format("map[string]{}", this->MapType(td->sub_type.get()));
}
std::string gen::GoGenerator::MapType(const til::ListSerializableTypeDef *td) {
  return fmt::format("[]{}", this->MapType(td->sub_type.get()));
}
