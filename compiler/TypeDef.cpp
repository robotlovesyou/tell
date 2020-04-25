#include "TypeDef.h"
bool til::TypeDef::optional() const {
  return optional_;
}

til::TypeDef::TypeDef(bool optional): optional_(optional) {
}
