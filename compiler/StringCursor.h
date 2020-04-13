//
// Created by ANDREW SMITH on 13/04/2020.
//

#ifndef TELL_COMPILER_STRINGCURSOR_H_
#define TELL_COMPILER_STRINGCURSOR_H_

#include <memory>
#include <string>
#include <sstream>
#include "CharCursor.h"

namespace til {
class StringCursor : public Cursor<char> {
 public:
  explicit StringCursor(const std::string& source);
  std::optional<char>Next() override;
  std::optional<const char *>Peek() override;
 private:
  std::istringstream stream_;
  std::unique_ptr<CharCursor> cursor_;
};
}

#endif //TELL_COMPILER_STRINGCURSOR_H_
