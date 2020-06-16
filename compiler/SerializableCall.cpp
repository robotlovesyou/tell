#include "SerializableCall.h"

static const char *K_NAME = "name";
static const char *K_DOC_COMMENT = "doc_comment";
static const char *K_ARGUMENT = "argument";
static const char *K_RETURNS = "returns";

void til::to_json(json &j, const SerializableCall &sc) {
  j = json{{K_NAME, sc.name}, {K_DOC_COMMENT, sc.doc_comment}, {K_ARGUMENT, sc.argument}, {K_RETURNS, sc.returns}};
}

void til::from_json(const json &j, SerializableCall &sc) {
  j.at(K_NAME).get_to(sc.name);
  j.at(K_DOC_COMMENT).get_to(sc.doc_comment);
  j.at(K_ARGUMENT).get_to(sc.argument);
  j.at(K_RETURNS).get_to(sc.returns);
}
