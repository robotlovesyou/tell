//
// Created by ANDREW SMITH on 13/04/2020.
//

#ifndef TELL_COMPILER_FILECURSOR_H_
#define TELL_COMPILER_FILECURSOR_H_
#include <exception>
#include <filesystem>
#include <memory>
#include <string>
#include <fstream>
#include "CharCursor.h"

namespace til {
class FileCursor : public Cursor<char> {
 public:

  class FileCursorException : public std::exception {
   public:
    explicit FileCursorException(std::filesystem::path path);
    const char *what();
   private:
    const std::filesystem::path path_;
  };

  explicit FileCursor(const std::filesystem::path &path);

  // explicitly delete copy constructor and copy assignment operator, move constructor and move assignment operator
  // to comply with the rule of 5. (an explicit destructor is defined)
  FileCursor(const FileCursor &) = delete;
  FileCursor &operator=(const FileCursor &) = delete;
  FileCursor(FileCursor &&) = delete;
  FileCursor &operator=(FileCursor &&) = delete;

  ~FileCursor();
  std::optional<char> Next() override;
  std::optional<const char *> Peek() override;
 private:
  std::ifstream stream_;
  std::unique_ptr<CharCursor> cursor_;

};
}

#endif //TELL_COMPILER_FILECURSOR_H_
