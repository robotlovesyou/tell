//
// Created by ANDREW SMITH on 25/04/2020.
//

#include "MessageTypeDef.h"
bool til::MessageTypeDef::optional() const {
  return false;
}
til::TypeDef::Type til::MessageTypeDef::t() const {
  return kList;
}
