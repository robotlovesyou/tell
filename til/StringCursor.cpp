//
// Created by ANDREW SMITH on 13/04/2020.
//

#include "StringCursor.h"

#include <memory>
til::StringCursor::StringCursor(std::string source): source_(std::move(source)) {}

std::optional<char> til::StringCursor::Next() {
  if (position_ < source_.size()) {
    return std::optional<char>(source_[position_++]);
  }
  return std::optional<char>();
}

std::optional<const char *> til::StringCursor::Peek() {
  if (position_ < source_.size()) {
    return std::optional<char*>(&source_[position_]);
  }
  return std::optional<char *>();
}
