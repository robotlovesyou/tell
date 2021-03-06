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
  std::string content_{""};
};
}

#endif //TELL_COMPILER_DOCCOMMENTCONTEXT_H_
