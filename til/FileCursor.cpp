#include "FileCursor.h"

#include <utility>
#include "fmt/core.h"

til::FileCursor::FileCursor(const std::string& path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    throw FileCursor::FileCursorException(path);
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  cursor_ = std::make_unique<StringCursor>(std::move(buffer.str()));
}

std::optional<char> til::FileCursor::Next() {
  return cursor_->Next();
}

std::optional<const char *> til::FileCursor::Peek() {
  return cursor_->Peek();
}

til::FileCursor::FileCursorException::FileCursorException(const std::string& path){
  message_ = fmt::format("File at {} not found", path);
}

const char *til::FileCursor::FileCursorException::what() const noexcept {
  return message_.c_str();
}
