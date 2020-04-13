//
// Created by ANDREW SMITH on 13/04/2020.
//

#include "StringCursor.h"

#include <memory>
til::StringCursor::StringCursor(const std::string& source) {
  stream_ = std::istringstream(source);
  cursor_ = std::make_unique<CharCursor>(&stream_);
}

std::optional<char> til::StringCursor::Next() {
  return cursor_->Next();
}

std::optional<const char *> til::StringCursor::Peek() {
  return cursor_->Peek();
}
