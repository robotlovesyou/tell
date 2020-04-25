//
// Created by ANDREW SMITH on 25/04/2020.
//

#include "MapTypeDef.h"
bool til::MapTypeDef::optional() const {
  return false;
}
til::TypeDef::Type til::MapTypeDef::t() const {
  return kList;
}
