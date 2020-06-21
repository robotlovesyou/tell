#ifndef TELL_COMPILER_STRINGCURSOR_H_
#define TELL_COMPILER_STRINGCURSOR_H_

#include <memory>
#include <string>
#include <sstream>
#include "Cursor.h"

namespace til {
/**
 * Cursor over a string source.
 * Implements the Cursor<char> interface
 */
class StringCursor : public Cursor<char> {
 public:
  /**
   * @param source The string for the cursor
   */
  explicit StringCursor(std::string source);

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

  // explicitly delete copy constructor and copy assignment operator, move constructor and move assignment operator
  // to encourage use of a unique_ptr
  // The default destructor is OK for this class.
  StringCursor(const StringCursor &) = delete;
  StringCursor &operator=(const StringCursor &) = delete;
  StringCursor(StringCursor &&) = delete;
  StringCursor &operator=(StringCursor &&) = delete;
 private:
  int position_{0};
  std::string source_;
};
}

#endif //TELL_COMPILER_STRINGCURSOR_H_
