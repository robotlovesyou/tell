#include "SerializableDirectiveDeclaration.h"

static const char *K_NAME = "name";
static const char *K_DOC_COMMENT = "doc_comment";
static const char *K_VALUE = "value";

void til::to_json(json &j, const SerializableDirectiveDeclaration &sdd) {
  j = json{{K_NAME, sdd.name}, {K_DOC_COMMENT, sdd.doc_comment}, {K_VALUE, sdd.value}};
}

void til::from_json(const json &j, SerializableDirectiveDeclaration &sdd) {
  j.at(K_NAME).get_to(sdd.name);
  j.at(K_DOC_COMMENT).get_to(sdd.doc_comment);
  j.at(K_VALUE).get_to(sdd.value);
}