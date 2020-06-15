#include "catch2/catch.hpp"
#include <iostream>
#include "SerializableTestHelpers.h"

TEST_CASE("Serialize and deserialize SerialisableMessageDeclaration") {
  auto smd = test_serializable_message_declaration();
  json j_md(*smd); // serialized to json
  std::cout << "Pretty Printed Serializable Message Declaration" << std::endl;
  std::cout <<j_md.dump(4) << std::endl;
  auto deserialized_md = j_md.get<til::SerializableMessageDeclaration>();
  CHECK(deserialized_md.name == "AMessageName");
  CHECK(deserialized_md.doc_comment == "Some docs");
  CHECK(deserialized_md.fields.size() == 2);
  auto *d_f1 = dynamic_cast<til::SerializableField *>(deserialized_md.fields[0].get());
  CHECK(d_f1->name == "field_a");
  CHECK(d_f1->doc_comment == "doc");
  auto *d_f2 = dynamic_cast<til::SerializableField *>(deserialized_md.fields[1].get());
  CHECK(d_f2->name == "field_b");
  CHECK(d_f2->doc_comment == "doc 2");
}