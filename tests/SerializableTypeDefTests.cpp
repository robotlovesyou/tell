#include "catch2/catch.hpp"
#include "nlohmann/json.hpp"
#include "../til/SerializableTypeDef.h"
#include <iostream>
#include <memory>

TEST_CASE("Serialize and Deserialize Scalar Type Def") {
  til::ScalarSerializableTypeDef s_td(til::ScalarTypeDef::kInt, true);
  json j_td(s_td); // serialized to json
  std::cout << "Pretty Printed Scalar Serialized Type Def" << std::endl;
  std::cout <<j_td.dump(4) << std::endl;
  auto deserialized_td = j_td.get<til::ScalarSerializableTypeDef>(); // deserialized from json
  CHECK(deserialized_td.scalar_type == til::ScalarTypeDef::kInt);
  CHECK(deserialized_td.is_optional);
}

TEST_CASE("Serialize and Deserialize Message Type Def") {
  til::MessageSerializableTypeDef m_td("MyMessage", true);
  json j_td(m_td); // serialized to json
  std::cout << "Pretty Printed Message Serialized Type Def" << std::endl;
  std::cout <<j_td.dump(4) << std::endl;
  auto deserialized_td = j_td.get<til::MessageSerializableTypeDef>();
  CHECK(deserialized_td.name == "MyMessage");
  CHECK(deserialized_td.is_optional);
}

TEST_CASE("Serialize and Deserialize List Type Def") {
  auto s_td = std::make_unique<til::ScalarSerializableTypeDef>(til::ScalarTypeDef::kTime, false);
  til::ListSerializableTypeDef l_td(std::move(s_td), true);
  json j_td(l_td); // serialized to json
  std::cout << "Pretty Printed List Serialized Type Def" << std::endl;
  std::cout <<j_td.dump(4) << std::endl;
  auto deserialized_td = j_td.get<til::ListSerializableTypeDef>();  // deserialized from json
  CHECK(deserialized_td.is_optional);
  CHECK(deserialized_td.sub_type->t() == til::TypeDef::kScalar);
  CHECK(dynamic_cast<til::ScalarSerializableTypeDef *>(deserialized_td.sub_type.get())->scalar_type == til::ScalarTypeDef::kTime);
}

TEST_CASE("Serialize and Deserialize Map Type Def") {
  auto s_td = std::make_unique<til::ScalarSerializableTypeDef>(til::ScalarTypeDef::kString, false);
  til::MapSerializableTypeDef m_td(std::move(s_td), true);
  json j_td(m_td); // serialized to json
  std::cout << "Pretty Printed Map Serialized Type Def" << std::endl;
  std::cout <<j_td.dump(4) << std::endl;
  auto deserialized_td = j_td.get<til::MapSerializableTypeDef>();  // deserialized from json
  CHECK(deserialized_td.is_optional);
  CHECK(deserialized_td.sub_type->t() == til::TypeDef::kScalar);
  CHECK(dynamic_cast<til::ScalarSerializableTypeDef *>(deserialized_td.sub_type.get())->scalar_type == til::ScalarTypeDef::kString);
}

TEST_CASE("Serialize and Deserialize Nested Map and List Type Defs") {
  auto msg_td = std::make_unique<til::MessageSerializableTypeDef>("MyMessage", false);
  auto l_td = std::make_unique<til::ListSerializableTypeDef>(std::move(msg_td), true);
  auto m_td = std::make_unique<til::MapSerializableTypeDef>(std::move(l_td), false);
  til::ListSerializableTypeDef top_td(std::move(m_td), true);
  json j_td(top_td); // serialized to json
  std::cout << "Pretty Printed Nested Serialized Type Def" << std::endl;
  std::cout <<j_td.dump(4) << std::endl;
  auto deserialized_td = j_td.get<til::ListSerializableTypeDef>();
  CHECK(deserialized_td.is_optional);
  auto *dm_td = dynamic_cast<til::MapSerializableTypeDef*>(deserialized_td.sub_type.get());
  CHECK_FALSE(dm_td->is_optional);
  auto *dl_td = dynamic_cast<til::ListSerializableTypeDef*>(dm_td->sub_type.get());
  CHECK(dl_td->is_optional);
  auto *dmsg_td = dynamic_cast<til::MessageSerializableTypeDef*>(dl_td->sub_type.get());
  CHECK(dmsg_td->name == "MyMessage");
  CHECK_FALSE(dmsg_td->is_optional);
}