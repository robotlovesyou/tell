#ifndef TELL_GEN_GENERATOR_H_
#define TELL_GEN_GENERATOR_H_

#include <string>
#include "SerializableAST.h"

namespace gen {
/**
 * Abstract class to be implemented by all code generator classes
 */
class Generator {
 public:
  virtual std::string Generate(const til::SerializableAST &ast) = 0;
};
}
#endif //TELL_GEN_GENERATOR_H_
