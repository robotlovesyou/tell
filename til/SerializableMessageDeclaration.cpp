//
// Created by ANDREW SMITH on 02/06/2020.
//

#include "SerializableMessageDeclaration.h"

static const char* K_NAME = "name";
static const char* K_DOC_COMMENT = "doc_comment";
static const char* K_FIELDS = "fields";

void til::to_json(json &j, const SerializableMessageDeclaration &smd) {
  j = json{{K_NAME, smd.name}, {K_DOC_COMMENT, smd.doc_comment}};
  for (const auto & field : smd.fields) {
    j[K_FIELDS].push_back(*field);
  }
}

void til::from_json(const json &j, SerializableMessageDeclaration &smd) {
  j.at(K_NAME).get_to(smd.name);
  j.at(K_DOC_COMMENT).get_to(smd.doc_comment);
  for (const auto &element : j.at(K_FIELDS)) {
    auto field = element.get<SerializableField>();
    auto unique_field = std::make_unique<SerializableField>();
    *unique_field = std::move(field);
    smd.fields.push_back(std::move(unique_field));
  }
}