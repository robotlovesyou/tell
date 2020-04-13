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
/**
 * Cursor over characters from a file source.
 * Implements RIAA for the owned ifstream by opening and closing it in the constructor/destructor
 * Implements the Cursor<char> interface via composition with the owned CharCursor object
 */
class FileCursor : public Cursor<char> {
 public:
  /**
   * Thrown by the FileCursor when it cannot open the requested file.
   */
  class FileCursorException : public std::exception {
   public:
    /**
     * @param path The requested file path
     */
    explicit FileCursorException(std::filesystem::path path);

    /**
     * Describe the exception
     * @return
     */
    const char *what();
   private:
    const std::filesystem::path path_;
  };

  /**
   * Open a file
   * @param path
   */
  explicit FileCursor(const std::filesystem::path &path);

  // explicitly delete copy constructor and copy assignment operator, move constructor and move assignment operator
  // to comply with the rule of 5.
  // An explicit destructor is defined but this class owns a stream so it should not be copied or moved.
  FileCursor(const FileCursor &) = delete;
  FileCursor &operator=(const FileCursor &) = delete;
  FileCursor(FileCursor &&) = delete;
  FileCursor &operator=(FileCursor &&) = delete;

  /**
   * Clean up the owned ifstream resource
   */
  ~FileCursor();

  /**
   * See Cursor for details
   * @return
   */
  std::optional<char> Next() override;

  /**
   * See Cursor for details
   * @return
   */
  std::optional<const char *> Peek() override;
 private:
  std::ifstream stream_;
  std::unique_ptr<CharCursor> cursor_;

};
}

#endif //TELL_COMPILER_FILECURSOR_H_
