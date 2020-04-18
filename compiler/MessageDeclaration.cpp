//
// Created by ANDREW SMITH on 18/04/2020.
//

#include "MessageDeclaration.h"

#include <stdexcept>
#include <memory>

#include "fmt/core.h"

til::Declaration::Type til::MessageDeclaration::t() {
  return kDirective;
}

std::string til::MessageDeclaration::name() {
  return std::string();
}

til::MessageDeclaration::MessageDeclaration(til::Token &&start_token, til::DocCommentContext &&doc, std::string name)
    : start_token_(std::move(start_token)), doc_(std::move(doc)), name_(std::move(name)) {

  if (start_token_.t!=Token::kMessage) {
    throw std::invalid_argument(fmt::format("MessageDeclaration start token cannot be a {}", start_token_.TypeName()));
  }
}

til::DocCommentContext til::MessageDeclaration::doc() {
  return til::DocCommentContext();
}

