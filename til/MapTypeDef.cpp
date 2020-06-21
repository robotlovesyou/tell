#include "MapTypeDef.h"
#include "SerializableTypeDef.h"
bool til::MapTypeDef::optional() const {
  return optional_;
}
til::TypeDef::Type til::MapTypeDef::t() const {
  return kMap;
}

til::MapTypeDef::MapTypeDef(std::unique_ptr<TypeDef> sub_type, bool optional): TypeDef(optional), SubTypeDef(std::move(sub_type)) {

}

std::unique_ptr<til::SerializableTypeDef> til::MapTypeDef::ToSerializable() const {
  return std::make_unique<MapSerializableTypeDef>(std::move(sub_type()->ToSerializable()), optional());
}
