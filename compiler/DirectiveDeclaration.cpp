//
// Created by ANDREW SMITH on 21/04/2020.
//

#include "DirectiveDeclaration.h"
til::DirectiveDeclaration::DirectiveDeclaration(til::Token &&startToken,
                                                til::DocCommentContext &&doc,
                                                std::string name,
                                                std::string value,
                                                std::shared_ptr<AST> ast) {

}

til::Declaration::Type til::DirectiveDeclaration::t() {
  return kDirective;
}

std::string til::DirectiveDeclaration::name() {
  return std::string();
}

std::shared_ptr<til::AST> til::DirectiveDeclaration::ast() {
  return std::shared_ptr<til::AST>();
}
