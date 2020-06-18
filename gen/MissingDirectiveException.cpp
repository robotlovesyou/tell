//
// Created by ANDREW SMITH on 18/06/2020.
//

#include "MissingDirectiveException.h"
#include "fmt/format.h"

const char *gen::MissingDirectiveException::what() const noexcept {
  return message_.c_str();
}

gen::MissingDirectiveException::MissingDirectiveException(const std::string& message) {
  message_ = fmt::format("Missing directive: {}", message);
}
