//
// Created by ANDREW SMITH on 16/06/2020.
//

#include "Compiler.h"
Compiler::Compiler(std::string f, std::string o): file_(std::move(f)), out_(std::move(o)) {
}

bool Compiler::Compile() {
  // open the file for reading
  // open the out for writing
  // create a file cursor for the source
  // create an error reporter
  // create a lexer
  // create a parser
  // parse the file
  // if the error reporter does not report any errors
  //    serialize the ast to the out file
  //    return true
  // otherwise
  //    return false
}
