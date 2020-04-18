//
// Created by ANDREW SMITH on 18/04/2020.
//

#ifndef TELL_COMPILER_DOCCOMMENTCONTEXT_H_
#define TELL_COMPILER_DOCCOMMENTCONTEXT_H_

#include <optional>
#include <string>

namespace til {
class DocCommentContext {
 public:
  /**
   * Return true if the DocCommentContext has had some content assigned
   * @return
   */
  [[nodiscard]] bool has_content() const noexcept;

  /**
   * Same as has_content
   * @return
   */
  explicit operator bool() const noexcept;

  /**
   * Return the content of the DocCommentContext.
   * Behaviour is undefined if the DocCommentContext has no content
   * @return
   */
  [[nodiscard]] const std::string & operator*() const;

  /**
   * Add a new line to the DocCommentContext
   * @param str
   * @return
   */
  DocCommentContext& operator +=(const std::string& str);
 private:
  std::optional<std::string> content_{};
};
}

#endif //TELL_COMPILER_DOCCOMMENTCONTEXT_H_
