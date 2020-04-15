//
// Created by ANDREW SMITH on 13/04/2020.
//

#include "FileCursor.h"

#include <utility>
#include "fmt/core.h"

til::FileCursor::FileCursor(const std::filesystem::path& path) {
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

til::FileCursor::FileCursorException::FileCursorException(std::filesystem::path path): path_(std::move(path)) {
}

const char *til::FileCursor::FileCursorException::what() {
  return fmt::format("File at {} not found", path_.string()).c_str();
}
