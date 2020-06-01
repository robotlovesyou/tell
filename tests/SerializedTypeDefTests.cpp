#include "catch2/catch.hpp"
#include "nlohmann/json.hpp"
#include "../compiler/SerializedTypeDef.h"
#include <iostream>
#include <memory>

TEST_CASE("Serialize type defs") {
//  auto t1 = std::make_unique<til::ScalarSerializedTypeDef>();
//  til::ScalarSerializedTypeDef t1{til::ScalarTypeDef::kInt, true};

  auto t4_ptr = std::make_unique<til::ScalarSerializedTypeDef>(til::ScalarTypeDef::kString, false);
  auto t3_ptr = std::make_unique<til::MapSerializedTypeDef>(std::move(t4_ptr), true);
  til::MapSerializedTypeDef t2(std::move(t3_ptr), false);
//  json j_t1(t1);
//  std::cout << j_t1.dump(4) << std::endl;
  json j_t2(t2);
  std::cout << j_t2.dump(4) << std::endl;
//  auto t1_2 = j_t1.get<til::ScalarSerializedTypeDef>();
//  std::cout << t1_2.t() << " " << t1_2.scalar_type << " " << t1_2.is_optional << " " << std::endl;
  try {
    auto t2_2 = j_t2.get<til::MapSerializedTypeDef>();
    std::cout << t2_2.t() << " " << t2_2.is_optional << " " << t2_2.sub_type->t() << " " << t2_2.sub_type->is_optional << std::endl;
    auto *mstd = dynamic_cast<til::MapSerializedTypeDef*>(t2_2.sub_type.get());
    std::cout << mstd->sub_type->t() << std::endl;
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }

}

TEST_CASE("Serialize and Deserialize Scalar Type Def") {
  til::ScalarSerializedTypeDef s_td(til::ScalarTypeDef::kInt, true);
  json j_td(s_td); // serialized to json
  std::cout << "Pretty Printed Scalar Serialized Type Def" << std::endl;
  std::cout <<j_td.dump(4) << std::endl;
  auto deserialized_td = j_td.get<til::ScalarSerializedTypeDef>(); // deserialized from json

  CHECK(deserialized_td.scalar_type == til::ScalarTypeDef::kInt);
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
  FAIL("pending");
}