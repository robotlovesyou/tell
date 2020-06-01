//
// Created by ANDREW SMITH on 31/05/2020.
//

#ifndef TELL_COMPILER_SERIALIZEDTYPEDEF_H_
#define TELL_COMPILER_SERIALIZEDTYPEDEF_H_

#include <memory>
#include "nlohmann/json.hpp"

#include "TypeDef.h"
#include "ScalarTypeDef.h"

using nlohmann::json;

namespace til {
struct SerializedTypeDef {
  virtual ~SerializedTypeDef() = default;
  SerializedTypeDef() = default;
  SerializedTypeDef(bool opt) : is_optional(opt) {}
  [[nodiscard]] virtual TypeDef::Type t() const = 0;
  bool is_optional{false};
};

struct ScalarSerializedTypeDef : public SerializedTypeDef {
  ScalarSerializedTypeDef() = default;
  ScalarSerializedTypeDef(ScalarTypeDef::ScalarType st, bool opt) : scalar_type(st), SerializedTypeDef(opt) {}
  [[nodiscard]] TypeDef::Type t() const override { return til::TypeDef::kScalar; }
  ScalarTypeDef::ScalarType scalar_type{ScalarTypeDef::kBool};
};

void to_json(json &j, const ScalarSerializedTypeDef &sstd);

void to_json(json &j, const std::unique_ptr<ScalarSerializedTypeDef> &sstd_ptr);

void from_json(const json &j, ScalarSerializedTypeDef &sstd);

struct ListSerializedTypeDef : public SerializedTypeDef {
  ListSerializedTypeDef() = default;
  ListSerializedTypeDef(std::unique_ptr<SerializedTypeDef> sub_t, bool opt)
      : sub_type(std::move(sub_t)), SerializedTypeDef(opt) {}
  [[nodiscard]] TypeDef::Type t() const override { return TypeDef::kList; }
  std::unique_ptr<SerializedTypeDef> sub_type;
};

void to_json(json &j, const ListSerializedTypeDef &lstd);

void to_json(json &j, const std::unique_ptr<ListSerializedTypeDef> &lstd_ptr);

void from_json(const json &j, ListSerializedTypeDef &lstd);

struct MapSerializedTypeDef : public SerializedTypeDef {
  MapSerializedTypeDef() = default;
  MapSerializedTypeDef(std::unique_ptr<SerializedTypeDef> sub_t, bool opt) : sub_type(std::move(sub_t)),
                                                                             SerializedTypeDef(opt) {}
  [[nodiscard]] TypeDef::Type t() const override {
    return TypeDef::kMap;
  }
  std::unique_ptr<SerializedTypeDef> sub_type;
};

void to_json(json &j, const MapSerializedTypeDef &mstd);

void to_json(json &j, const std::unique_ptr<MapSerializedTypeDef> &mstd_ptr);

void from_json(const json &j, MapSerializedTypeDef &mstd);
}

#endif //TELL_COMPILER_SERIALIZEDTYPEDEF_H_
