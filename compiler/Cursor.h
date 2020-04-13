//
// Created by ANDREW SMITH on 13/04/2020.
//

#ifndef TELL_COMPILER_CURSOR_H_
#define TELL_COMPILER_CURSOR_H_

#include <memory>
#include <optional>

namespace til {

template <class T>
class Cursor {
 public:
  virtual std::optional<T>Next() = 0;
  virtual std::optional<const T*> Peek() = 0;
};

} // namespace til

#endif //TELL_COMPILER_CURSOR_H_
