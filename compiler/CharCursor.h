//
// Created by ANDREW SMITH on 13/04/2020.
//

#ifndef TELL_COMPILER_CHARCURSOR_H_
#define TELL_COMPILER_CHARCURSOR_H_

#import "Cursor.h"
#import <istream>

namespace til {
class CharCursor: public Cursor<char> {
 public:
  std::optional<char>Next() override;
  std::optional<const char*> Peek() override;
  explicit CharCursor(std::istream *source);
 private:
  char next_{0};
  bool eof_{false};
  std::istream *source_;
};
}

#endif //TELL_COMPILER_CHARCURSOR_H_
