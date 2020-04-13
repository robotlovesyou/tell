//
// Created by ANDREW SMITH on 13/04/2020.
//

#ifndef TELL_COMPILER_CHARCURSOR_H_
#define TELL_COMPILER_CHARCURSOR_H_

#import "Cursor.h"
#import <istream>

namespace til {
/**
 * Class for iterating over istream sources.
 * Implements the Cursor<char> interface.
 * Is consumed by StringCursor and FileCursor to allow them
 * to implement Cursor<char> by composition.
 */
class CharCursor: public Cursor<char> {
 public:
  /**
   * See Cursor for details
   * @return
   */
  std::optional<char>Next() override;

  /**
   * See Cursor for details
   * @return
   */
  std::optional<const char*> Peek() override;

  /**
   * Construct a cursor over the provided stream
   * @param source
   */
  explicit CharCursor(std::istream *source);

 private:
  char next_{0};
  bool eof_{false};
  std::istream *source_;
};
}

#endif //TELL_COMPILER_CHARCURSOR_H_
