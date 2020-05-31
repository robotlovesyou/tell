//
// Created by ANDREW SMITH on 31/05/2020.
//

#ifndef TELL_COMPILER_SERIALIZEDTYPEDEF_H_
#define TELL_COMPILER_SERIALIZEDTYPEDEF_H_

#include <memory>
#include "json/json.hpp"

#include "TypeDef.h"
#include "ScalarTypeDef.h"

using nlohmann::json;
namespace til {
struct SerializedTypeDef {
  virtual ~SerializedTypeDef() = default;
  SerializedTypeDef() = default;
  SerializedTypeDef(bool opt): is_optional(opt) {}
  [[nodiscard]] virtual TypeDef::Type t() const = 0;
  bool is_optional{false};
};

struct ScalarSerializedTypeDef : public SerializedTypeDef {
  ScalarSerializedTypeDef() = default;
  ScalarSerializedTypeDef(ScalarTypeDef::ScalarType st, bool opt) : scalar_type(st), SerializedTypeDef(opt) {}
  [[nodiscard]] TypeDef::Type t() const override {
    return TypeDef::kScalar;
  }
  ScalarTypeDef::ScalarType scalar_type{ScalarTypeDef::kBool};
};

void to_json(json &j, const ScalarSerializedTypeDef &sstd) {
  j = json{{"t", sstd.t()}, {"scalar_type", sstd.scalar_type}, {"is_optional", sstd.is_optional}};
}

void to_json(json &j, const std::unique_ptr<ScalarSerializedTypeDef> &sstd_ptr) {
  to_json(j, *sstd_ptr);
}

void from_json(const json &j, ScalarSerializedTypeDef &sstd) {
  j.at("scalar_type").get_to(sstd.scalar_type);
  j.at("is_optional").get_to(sstd.is_optional);
}

struct MapSerializedTypeDef : public SerializedTypeDef {
  MapSerializedTypeDef() = default;
  MapSerializedTypeDef(std::unique_ptr<SerializedTypeDef> sub_t, bool opt) : sub_type(std::move(sub_t)),
                                                                                      SerializedTypeDef(opt) {}
  [[nodiscard]] TypeDef::Type t() const override {
    return TypeDef::kMap;
  }
  std::unique_ptr<SerializedTypeDef> sub_type;
};

void to_json(json &j, const MapSerializedTypeDef &mstd) {
  j["t"] = mstd.t();
  j["is_optional"] = mstd.is_optional;
  switch (mstd.sub_type->t()) {

    case TypeDef::kScalar: {
      const auto *st = dynamic_cast<const ScalarSerializedTypeDef *>(mstd.sub_type.get());
      j["sub_type"] = *st;
      break;
    }
    case TypeDef::kMessage:break;
    case TypeDef::kMap: {
      const auto *mt = dynamic_cast<const MapSerializedTypeDef *>(mstd.sub_type.get());
      j["sub_type"] = *mt;
      break;
    }
    case TypeDef::kList:break;
  }
}

void to_json(json &j, const std::unique_ptr<MapSerializedTypeDef> &mstd_ptr) {
  to_json(j, *mstd_ptr);
}

void from_json(const json &j, MapSerializedTypeDef &mstd) {
  j.at("is_optional").get_to(mstd.is_optional);
  auto sub_t = j.at("sub_type").at("t").get<TypeDef::Type>();
  switch (sub_t) {
    case TypeDef::kScalar: {
      auto sstd = j.at("sub_type").get<ScalarSerializedTypeDef>();
      auto sub_type = std::make_unique<ScalarSerializedTypeDef>();
      *sub_type = std::move(sstd);
      mstd.sub_type = std::move(sub_type);
      break;
    }
    case TypeDef::kMap: {
      auto map_std = j.at("sub_type").get<MapSerializedTypeDef>();
      auto sub_type = std::make_unique<MapSerializedTypeDef>();
      *sub_type = std::move(map_std);
      mstd.sub_type = std::move(sub_type);
      break;
    }
    case TypeDef::kList:break;
    case TypeDef::kMessage:break;
  }
}
}

#endif //TELL_COMPILER_SERIALIZEDTYPEDEF_H_
