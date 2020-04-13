//
// Created by ANDREW SMITH on 13/04/2020.
//

#include "FileCursor.h"

#include <utility>
#include "fmt/core.h"

til::FileCursor::FileCursor(const std::filesystem::path& path) {
  stream_ = std::ifstream(path);
  if (!stream_.is_open()) {
    throw FileCursor::FileCursorException(path);
  }
  cursor_ = std::make_unique<CharCursor>(&stream_);
}

til::FileCursor::~FileCursor() {
  // Close the file resource
  stream_.close();
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
