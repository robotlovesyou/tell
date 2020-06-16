#ifndef TELL_COMPILER_COMPILER_H_
#define TELL_COMPILER_COMPILER_H_

#include <memory>
#include <string>

class Compiler {
 public:
  Compiler(std::string f, std::string o);
  bool Compile();
 private:
  std::string file_;
  std::string out_;
};

#endif //TELL_COMPILER_COMPILER_H_
