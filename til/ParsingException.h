//
// Created by ANDREW SMITH on 24/04/2020.
//

#ifndef TELL_COMPILER_PARSINGEXCEPTION_H_
#define TELL_COMPILER_PARSINGEXCEPTION_H_

#include <exception>
#include <string>

namespace til {
 class ParsingException: public std::exception {
  public:
   explicit ParsingException(std::string message);
   [[nodiscard]] const char* what() const noexcept override;
  private:
   std::string message_;
};
}

#endif //TELL_COMPILER_PARSINGEXCEPTION_H_