#include "catch2/catch.hpp"
#include "json/json.hpp"
#include "../compiler/SerializedTypeDef.h"
#include <iostream>
#include <vector>
#include <memory>

TEST_CASE("Serialize type defs") {
  auto t1 = std::make_unique<til::ScalarSerializedTypeDef>();
  t1->scalar_type = til::ScalarTypeDef::kInt;
  t1->is_optional = true;
  auto t2 = std::make_unique<til::MapSerializedTypeDef>();
  auto t3 = std::make_unique<til::ScalarSerializedTypeDef>();
  t3->scalar_type = til::ScalarTypeDef::kFloat;
  t3->is_optional = true;
  t2->sub_type = std::move(t3);
  json j_t1(t1);
  std::cout << j_t1.dump() << std::endl;
  json j_t2(t2);
  std::cout << j_t2.dump() << std::endl;
  auto t1_2 = j_t1.get<til::ScalarSerializedTypeDef>();
  std::cout << t1_2.t() << " " << t1_2.scalar_type << " " << t1_2.is_optional << " " << std::endl;
  try {
    auto t2_2 = j_t2.get<til::MapSerializedTypeDef>();
    std::cout << t2_2.t() << " " << t2_2.is_optional << " " << t2_2.sub_type->t() << " " << t2_2.sub_type->is_optional << std::endl;
    auto *sstd = dynamic_cast<til::ScalarSerializedTypeDef*>(t2_2.sub_type.get());
    std::cout << sstd->scalar_type << std::endl;
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }

}