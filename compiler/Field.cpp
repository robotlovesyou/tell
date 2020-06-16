#include "Field.h"

#include <utility>

#include "SerializableField.h"

std::string til::Field::name() const {
  return name_;
}

const til::TypeDef *til::Field::type_def() const {
  return type_def_.get();
}

til::Field::Field(std::string name,
                  std::unique_ptr<til::TypeDef> type_def,
                  std::unique_ptr<DocCommentContext> doc)
    : name_(std::move(name)), type_def_(std::move(type_def)), doc_(std::move(doc)) {
}

const til::DocCommentContext &til::Field::doc() const {
  return *doc_;
}

std::unique_ptr<til::SerializableField> til::Field::ToSerializable() const {
  return std::make_unique<SerializableField>(name_, doc_->content(), type_def_->ToSerializable());
}
