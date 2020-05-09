#include "MessageTypeDef.h"
bool til::MessageTypeDef::optional() const {
  return optional_;
}
til::TypeDef::Type til::MessageTypeDef::t() const {
  return kMessage;
}

til::MessageTypeDef::MessageTypeDef(std::string name, const std::shared_ptr<AST>& ast, bool optional): name_(std::move(name)), ast_(ast), TypeDef(optional) {
}
const std::string &til::MessageTypeDef::name() const {
  return name_;
}
