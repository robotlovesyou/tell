#include "Argument.h"
til::Argument::Argument(std::string name, const std::shared_ptr<AST> &ast): name_(std::move(name)), ast_(ast) {
}

std::string til::Argument::name() const {
  return name_;
}
