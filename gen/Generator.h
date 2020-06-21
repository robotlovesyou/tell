#ifndef TELL_GEN_GENERATOR_H_
#define TELL_GEN_GENERATOR_H_

#import <string>
#import "SerializableAST.h"

namespace gen {
class Generator {
 public:
  virtual std::string Generate(const til::SerializableAST &ast) = 0;
};
}
#endif //TELL_GEN_GENERATOR_H_
