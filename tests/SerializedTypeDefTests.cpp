#include "catch2/catch.hpp"
#include "json/json.hpp"
#include "../compiler/SerializedTypeDef.h"
#include <iostream>
#include <vector>
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