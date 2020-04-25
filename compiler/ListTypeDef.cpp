//
// Created by ANDREW SMITH on 25/04/2020.
//

#include "ListTypeDef.h"
bool til::ListTypeDef::optional() const {
  return false;
}
til::TypeDef::Type til::ListTypeDef::t() const {
  return kList;
}
