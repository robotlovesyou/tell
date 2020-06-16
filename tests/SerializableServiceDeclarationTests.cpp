#include "catch2/catch.hpp"
#include "SerializableTestHelpers.h"

#include <iostream>

TEST_CASE("Serialize and deserialize SerializableServiceDeclaration") {
  auto sd = test_serializable_service_declaration("AService", "Some docs");
  json j(*sd);
  std::cout << "Pretty Printed Serializable Message Declaration" << std::endl;
  std::cout <<j.dump(4) << std::endl;
  auto deserialized_sd = j.get<til::SerializableServiceDeclaration>();
  CHECK(deserialized_sd.name == "AService");
  CHECK(deserialized_sd.doc_comment == "Some docs");
  CHECK(deserialized_sd.calls.size() == sd->calls.size());
}

