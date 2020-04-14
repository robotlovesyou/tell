//
// Created by ANDREW SMITH on 13/04/2020.
//

#ifndef TELL_COMPILER_CURSOR_H_
#define TELL_COMPILER_CURSOR_H_

#include <memory>
#include <optional>

namespace til {

/**
 * Interface for cursor classes
 * @tparam T
 */
template <class T>
class Cursor {
 public:

  /**
   * Declare virtual constructor to prevent warning from -Wdelete-abstract-non-virtual-dtor
   */
  virtual ~Cursor() = default;
  /**
   * Return the next value if it exists
   * @return the next value or an empty option
   */
  virtual std::optional<T>Next() = 0;

  /**
   * Peek at the next value without consuming it.
   * @return a const pointer tothe next value or an empty option
   */
  virtual std::optional<const T*> Peek() = 0;
};

} // namespace til

#endif //TELL_COMPILER_CURSOR_H_
