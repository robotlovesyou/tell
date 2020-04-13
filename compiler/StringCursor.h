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
/**
 * Cursor over a string source.
 * Implements the Cursor<char> interface via composition with the owned CharCursor object
 */
class StringCursor : public Cursor<char> {
 public:
  /**
   * @param source The string for the cursor
   */
  explicit StringCursor(const std::string& source);

  /**
   * See Cursor for details
   * @return
   */
  std::optional<char>Next() override;

  /**
   * See Cursor for details
   * @return
   */
  std::optional<const char *>Peek() override;
 private:
  std::istringstream stream_;
  std::unique_ptr<CharCursor> cursor_;
};
}

#endif //TELL_COMPILER_STRINGCURSOR_H_
