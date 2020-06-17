#include "DocCommentContext.h"
bool til::DocCommentContext::has_content() const noexcept {
  return !content_.empty();
}

const std::string &til::DocCommentContext::content() const {
  return content_;
}

void til::DocCommentContext::append(const std::string &str) {
  if (content_.empty()) {
    content_ = str;
  } else {
    content_ += "\n";
    content_ += str;
  }
}

