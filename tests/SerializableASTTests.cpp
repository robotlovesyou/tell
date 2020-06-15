#include "catch2/catch.hpp"
#include <iostream>
#include "SerializableTestHelpers.h"
#include "../compiler/SerializableAST.h"

TEST_CASE("Serialize and deserialize SerializableAST") {
  std::vector<std::unique_ptr<til::SerializableDirectiveDeclaration>> directive_decls;
  directive_decls.push_back(std::move(std::make_unique<til::SerializableDirectiveDeclaration>("name_one", "value_one")));
  directive_decls.push_back(std::move(std::make_unique<til::SerializableDirectiveDeclaration>("name_two", "value_two")));

  std::vector<std::unique_ptr<til::SerializableMessageDeclaration>> message_decls;
  message_decls.push_back(std::move(test_serializable_message_declaration("MessageNameOne")));
  message_decls.push_back(std::move(test_serializable_message_declaration("MessageNameTwo")));
  til::SerializableAST ast(std::move(directive_decls), std::move(message_decls));

  json j_ast(ast);
  std::cout << "Pretty Printed Serializable AST" << std::endl;
  std::cout <<j_ast.dump(4) << std::endl;

  auto deserialized_ast = j_ast.get<til::SerializableAST>();

  CHECK(deserialized_ast.directive_declarations.size() == 2);
  CHECK(deserialized_ast.directive_declarations[0]->name == "name_one");
  CHECK(deserialized_ast.directive_declarations[1]->name == "name_two");

  CHECK(deserialized_ast.message_declarations.size() == 2);
  CHECK(deserialized_ast.message_declarations[0]->name == "MessageNameOne");
  CHECK(deserialized_ast.message_declarations[1]->name == "MessageNameTwo");
}

