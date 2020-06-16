#include "catch2/catch.hpp"
#include <iostream>
#include "../til/SerializableField.h"

TEST_CASE("Serialize and Deserialize Field") {
  const char * FIELD_NAME = "a_field";
  const char * FIELD_DOCS = "some docs for the field";

  std::unique_ptr<til::SerializableTypeDef>
      td = std::move(std::make_unique<til::ScalarSerializableTypeDef>(til::ScalarTypeDef::kTime, true));

  til::SerializableField fld(FIELD_NAME, FIELD_DOCS, std::move(td));
  json j_fld(fld);
  std::cout << "Pretty Printed Serializable Field" << std::endl;
  std::cout <<j_fld.dump(4) << std::endl;
  auto deserialized_fld = j_fld.get<til::SerializableField>();
  CHECK(deserialized_fld.name == FIELD_NAME);
  CHECK(deserialized_fld.doc_comment == FIELD_DOCS);
  auto d_td = dynamic_cast<til::ScalarSerializableTypeDef *>(deserialized_fld.type_def.get());
  CHECK(d_td->scalar_type == til::ScalarTypeDef::kTime);
  CHECK(d_td->is_optional);
}