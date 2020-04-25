//
// Created by ANDREW SMITH on 25/04/2020.
//

#include "ScalarTypeDef.h"
til::TypeDef::Type til::ScalarTypeDef::t() const {
  return kScalar;
}

til::ScalarTypeDef::ScalarTypeDef(til::ScalarTypeDef::ScalarType st, bool optional): TypeDef(optional), scalar_type_(st) {
}

til::ScalarTypeDef::ScalarType til::ScalarTypeDef::scalar_type() const {
  return scalar_type_;
}
