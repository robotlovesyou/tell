#include "SubTypeDef.h"

til::SubTypeDef::SubTypeDef(std::unique_ptr<TypeDef> sub_type): sub_type_(std::move(sub_type)) {
}
const til::TypeDef *til::SubTypeDef::sub_type() const {
  return sub_type_.get();
}
