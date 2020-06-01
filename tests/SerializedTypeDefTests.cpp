#include "catch2/catch.hpp"
#include "nlohmann/json.hpp"
#include "../compiler/SerializedTypeDef.h"
#include <iostream>
#include <memory>

TEST_CASE("Serialize and Deserialize Scalar Type Def") {
  til::ScalarSerializedTypeDef s_td(til::ScalarTypeDef::kInt, true);
  json j_td(s_td); // serialized to json
  std::cout << "Pretty Printed Scalar Serialized Type Def" << std::endl;
  std::cout <<j_td.dump(4) << std::endl;
  auto deserialized_td = j_td.get<til::ScalarSerializedTypeDef>(); // deserialized from json
  CHECK(deserialized_td.scalar_type == til::ScalarTypeDef::kInt);
  CHECK(deserialized_td.is_optional);
}

TEST_CASE("Serialize and Deserialize Message Type Def") {
  til::MessageSerializedTypeDef m_td("MyMessage", true);
  json j_td(m_td); // serialized to json
  std::cout << "Pretty Printed Message Serialized Type Def" << std::endl;
  std::cout <<j_td.dump(4) << std::endl;
  auto deserialized_td = j_td.get<til::MessageSerializedTypeDef>();
  CHECK(deserialized_td.name == "MyMessage");
  CHECK(deserialized_td.is_optional);
}

TEST_CASE("Serialize and Deserialize List Type Def") {
  auto s_td = std::make_unique<til::ScalarSerializedTypeDef>(til::ScalarTypeDef::kTime, false);
  til::ListSerializedTypeDef l_td(std::move(s_td), true);
  json j_td(l_td); // serialized to json
  std::cout << "Pretty Printed List Serialized Type Def" << std::endl;
  std::cout <<j_td.dump(4) << std::endl;
  auto deserialized_td = j_td.get<til::ListSerializedTypeDef>();  // deserialized from json
  CHECK(deserialized_td.is_optional);
  CHECK(deserialized_td.sub_type->t() == til::TypeDef::kScalar);
  CHECK(dynamic_cast<til::ScalarSerializedTypeDef *>(deserialized_td.sub_type.get())->scalar_type == til::ScalarTypeDef::kTime);
}

TEST_CASE("Serialize and Deserialize Map Type Def") {
  auto s_td = std::make_unique<til::ScalarSerializedTypeDef>(til::ScalarTypeDef::kString, false);
  til::MapSerializedTypeDef m_td(std::move(s_td), true);
  json j_td(m_td); // serialized to json
  std::cout << "Pretty Printed Map Serialized Type Def" << std::endl;
  std::cout <<j_td.dump(4) << std::endl;
  auto deserialized_td = j_td.get<til::MapSerializedTypeDef>();  // deserialized from json
  CHECK(deserialized_td.is_optional);
  CHECK(deserialized_td.sub_type->t() == til::TypeDef::kScalar);
  CHECK(dynamic_cast<til::ScalarSerializedTypeDef *>(deserialized_td.sub_type.get())->scalar_type == til::ScalarTypeDef::kString);
}

TEST_CASE("Serialize and Deserialize Nested Map and List Type Defs") {
  auto msg_td = std::make_unique<til::MessageSerializedTypeDef>("MyMessage", false);
  auto l_td = std::make_unique<til::ListSerializedTypeDef>(std::move(msg_td), true);
  auto m_td = std::make_unique<til::MapSerializedTypeDef>(std::move(l_td), false);
  til::ListSerializedTypeDef top_td(std::move(m_td), true);
  json j_td(top_td); // serialized to json
  std::cout << "Pretty Printed Nested Serialized Type Def" << std::endl;
  std::cout <<j_td.dump(4) << std::endl;
  auto deserialized_td = j_td.get<til::ListSerializedTypeDef>();
  CHECK(deserialized_td.is_optional);
  auto *dm_td = dynamic_cast<til::MapSerializedTypeDef*>(deserialized_td.sub_type.get());
  CHECK_FALSE(dm_td->is_optional);
  auto *dl_td = dynamic_cast<til::ListSerializedTypeDef*>(dm_td->sub_type.get());
  CHECK(dl_td->is_optional);
  auto *dmsg_td = dynamic_cast<til::MessageSerializedTypeDef*>(dl_td->sub_type.get());
  CHECK(dmsg_td->name == "MyMessage");
  CHECK_FALSE(dmsg_td->is_optional);
}