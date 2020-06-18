#include "GoGenerator.h"
#include "MissingDirectiveException.h"
std::string gen::GoGenerator::Generate(til::SerializableAST &ast) {
  // ensure that the ast has a go_package directive and a go_path directive
  EnsureDirective(ast, "go_path");
  EnsureDirective(ast, "go_package");
  return std::string();
}

void gen::GoGenerator::EnsureDirective(til::SerializableAST &ast, const std::string& name) {
  for (const std::unique_ptr<til::SerializableDirectiveDeclaration> & sdd: ast.directive_declarations) {
    if(sdd->name == name) {
      return;
    }
  }
  throw MissingDirectiveException(name);
}
