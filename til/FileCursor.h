#ifndef TELL_COMPILER_FILECURSOR_H_
#define TELL_COMPILER_FILECURSOR_H_
#include <exception>
#include <experimental/filesystem>
#include <memory>
#include <string>
#include <fstream>
#include "StringCursor.h"

namespace til {
/**
 * Cursor over characters from a file source.
 * Implements the Cursor<char> interface via composition with the owned StringCursor object
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
    explicit FileCursorException(const std::experimental::filesystem::path& path);

    /**
     * Describe the exception
     * @return
     */
    [[nodiscard]] const char *what() const noexcept override ;
   private:
    std::string message_;
  };

  /**
   * Open a file
   * @param path
   */
  explicit FileCursor(const std::experimental::filesystem::path &path);

  // explicitly delete copy constructor and copy assignment operator, move constructor and move assignment operator
  FileCursor(const FileCursor &) = delete;
  FileCursor &operator=(const FileCursor &) = delete;
  FileCursor(FileCursor &&) = delete;
  FileCursor &operator=(FileCursor &&) = delete;

  // Rule of 5. Explicitly declare the default destructor to be fine
  ~FileCursor() override = default;

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
  std::unique_ptr<StringCursor> cursor_;

};
}

#endif //TELL_COMPILER_FILECURSOR_H_
