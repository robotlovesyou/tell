//
// Created by ANDREW SMITH on 25/04/2020.
//

#include "ListTypeDef.h"
#include "SerializableTypeDef.h"
bool til::ListTypeDef::optional() const {
  return optional_;
}
til::TypeDef::Type til::ListTypeDef::t() const {
  return kList;
}

til::ListTypeDef::ListTypeDef(std::unique_ptr<TypeDef> sub_type, bool optional)
    : TypeDef(optional), SubTypeDef(std::move(sub_type)) {
}

std::unique_ptr<til::SerializableTypeDef> til::ListTypeDef::ToSerializable() const {
  return std::make_unique<ListSerializableTypeDef>(std::move(sub_type()->ToSerializable()), optional());
}
