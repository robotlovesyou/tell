#include "catch2/catch.hpp"
#include <iostream>
#include "SerializableDirectiveDeclaration.h"

TEST_CASE("Serialize and deserialize SerializableDirectiveDeclaration") {
  til::SerializableDirectiveDeclaration sdd("a_name", "Some docs", "a_value");
  json j_sdd(sdd);
  std::cout << "Pretty Printed Serializable Directive Declaration" << std::endl;
  std::cout <<j_sdd.dump(4) << std::endl;
  auto deserialized_sdd = j_sdd.get<til::SerializableDirectiveDeclaration>();
  CHECK(deserialized_sdd.name == "a_name");
  CHECK(deserialized_sdd.doc_comment == "Some docs");
  CHECK(deserialized_sdd.value == "a_value");
}
