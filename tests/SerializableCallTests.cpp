#include "catch2/catch.hpp"
#include "../compiler/SerializableCall.h"

#include <iostream>

TEST_CASE("Serialize and deserialize SerializableCall") {
  til::SerializableCall sc("call_name", "doc comment", "ArgMessage", "RetMessage");
  json j(sc);
  std::cout << "Pretty Printed Serializable Call" << std::endl;
  std::cout <<j.dump(4) << std::endl;
  auto deserialized_call = j.get<til::SerializableCall>();
  CHECK(deserialized_call.name == "call_name");
  CHECK(deserialized_call.doc_comment == "doc comment");
  CHECK(deserialized_call.argument == "ArgMessage");
  CHECK(deserialized_call.returns == "RetMessage");
}
