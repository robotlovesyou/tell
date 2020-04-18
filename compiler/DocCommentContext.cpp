#include "DocCommentContext.h"
bool til::DocCommentContext::has_content() const noexcept {
  return content_.has_value();
}

til::DocCommentContext::operator bool() const noexcept {
  return this->has_content();
}

til::DocCommentContext &til::DocCommentContext::operator+=(const std::string& str) {
  if(content_) {
    *content_ += "\n";
    *content_ += str;
  } else {
    content_ = str;
  }
  return *this;
}

const std::string &til::DocCommentContext::operator*() const {
  return content_.value();
}

