#include "Call.h"
#include "SerializableCall.h"

til::Call::Call(std::string name,
                std::unique_ptr<DocCommentContext> doc,
                std::unique_ptr<Argument> argument,
                std::unique_ptr<Argument> returns)
    : name_(std::move(name)), doc_(std::move(doc)), argument_(std::move(argument)), returns_(std::move(returns)) {
}

std::string til::Call::name() const {
  return name_;
}

const til::DocCommentContext &til::Call::doc() const {
  return *doc_;
}

const til::Argument &til::Call::argument() const {
  return *argument_;
}

const til::Argument &til::Call::returns() const {
  return *returns_;
}

std::unique_ptr<til::SerializableCall> til::Call::ToSerializable() const {
  return std::make_unique<SerializableCall>(name_, doc_->content(), argument_->name(), returns_->name());
}
