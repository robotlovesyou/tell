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
   * Return the content of the DocCommentContext.
   * Behaviour is undefined if the DocCommentContext has no content
   * @return
   */
  const std::string & content() const;

  /**
   * Add a new line to the DocCommentContext
   * @param str
   * @return
   */
  void append(const std::string& str);
 private:
  std::optional<std::string> content_{};
};
}

#endif //TELL_COMPILER_DOCCOMMENTCONTEXT_H_
