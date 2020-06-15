//
// Created by ANDREW SMITH on 01/06/2020.
//

#include "SerializableField.h"

static const char *K_NAME = "name";
static const char *K_TYPE_DEF = "type_def";
static const char *K_DOC_COMMENT = "doc_comment";

void til::to_json(json &j, const til::SerializableField &field) {
  j = json{{K_NAME, field.name}, {K_DOC_COMMENT, field.doc_comment}, {K_TYPE_DEF, field.type_def}};
}

void til::from_json(const json &j, til::SerializableField &field) {
  field.name = j.at(K_NAME).get<std::string>();
  field.doc_comment = j.at(K_DOC_COMMENT).get<std::string>();
  field.type_def = std::move(j.at(K_TYPE_DEF).get<std::unique_ptr<SerializableTypeDef>>());
}
