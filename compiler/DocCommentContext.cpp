#include "DocCommentContext.h"
bool til::DocCommentContext::has_content() const noexcept {
  return content_.has_value();
}

const std::string &til::DocCommentContext::content() const {
  if (content_.has_value()) {
    return content_.value();
  }
  return std::move(std::string(""));
}

void til::DocCommentContext::append(const std::string &str) {
  if(content_) {
    *content_ += "\n";
    *content_ += str;
  } else {
    content_ = str;
  }
}

