//
// Created by ANDREW SMITH on 25/04/2020.
//

#include "MapTypeDef.h"
bool til::MapTypeDef::optional() const {
  return optional_;
}
til::TypeDef::Type til::MapTypeDef::t() const {
  return kMap;
}

til::MapTypeDef::MapTypeDef(std::unique_ptr<TypeDef> sub_type, bool optional): TypeDef(optional), SubTypeDef(std::move(sub_type)) {

}
