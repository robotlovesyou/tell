//
// Created by ANDREW SMITH on 13/04/2020.
//

#include "CharCursor.h"
til::CharCursor::CharCursor(std::istream *source): source_(source) {
  eof_ = (*source_ >> next_).eof();
}

std::optional<char> til::CharCursor::Next() {
  if(eof_) {
    return std::optional<char>();
  }
  std::optional<char> out(next_);
  eof_ = (*source_ >> next_).eof();
  return out;
}

std::optional<const char *> til::CharCursor::Peek() {
  if(eof_) {
    return std::optional<char *>();
  }
  return std::optional<char *>(&next_);
}
