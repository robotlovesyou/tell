#include "DocCommentContext.h"
bool til::DocCommentContext::has_content() const noexcept {
  return content_.has_value();
}

const std::string &til::DocCommentContext::content() const {
  return content_.value();
}

void til::DocCommentContext::append(const std::string &str) {
  if(content_) {
    *content_ += "\n";
    *content_ += str;
  } else {
    content_ = str;
  }
}

