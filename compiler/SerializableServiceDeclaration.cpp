#include "SerializableServiceDeclaration.h"

static const char *K_NAME = "name";
static const char *K_DOC_COMMENT = "doc_comment";
static const char *K_CALLS = "calls";

void til::to_json(json &j, const SerializableServiceDeclaration &sd) {
  j[K_NAME] = sd.name;
  j[K_DOC_COMMENT] = sd.doc_comment;
  for (const auto & call : sd.calls) {
    j[K_CALLS].push_back(*call);
  }
}

void til::from_json(const json &j, SerializableServiceDeclaration &sd) {
  j.at(K_NAME).get_to(sd.name);
  j.at(K_DOC_COMMENT).get_to(sd.doc_comment);

  for (const auto & element : j.at(K_CALLS)) {
    auto call = element.get<SerializableCall>();
    auto unique_call = std::make_unique<SerializableCall>();
    *unique_call = std::move(call);
    sd.calls.push_back(std::move(unique_call));
  }
}