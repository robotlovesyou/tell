//
// Created by ANDREW SMITH on 31/05/2020.
//

#ifndef TELL_COMPILER_SERIALIZABLETYPEDEF_H_
#define TELL_COMPILER_SERIALIZABLETYPEDEF_H_

#include <memory>
#include "nlohmann/json.hpp"

#include "TypeDef.h"
#include "ScalarTypeDef.h"

using nlohmann::json;

namespace til {
struct SerializableTypeDef {
  virtual ~SerializableTypeDef() = default;
  SerializableTypeDef() = default;
  SerializableTypeDef(bool opt) : is_optional(opt) {}
  [[nodiscard]] virtual TypeDef::Type t() const = 0;
  bool is_optional{false};
};

void to_json(json &j, const SerializableTypeDef *td);

void from_json(const json &j, std::unique_ptr<SerializableTypeDef> &td);

struct ScalarSerializableTypeDef : public SerializableTypeDef {
  ScalarSerializableTypeDef() = default;
  ScalarSerializableTypeDef(ScalarTypeDef::ScalarType st, bool opt) : scalar_type(st), SerializableTypeDef(opt) {}
  [[nodiscard]] TypeDef::Type t() const override { return til::TypeDef::kScalar; }
  ScalarTypeDef::ScalarType scalar_type{ScalarTypeDef::kBool};
};

void to_json(json &j, const ScalarSerializableTypeDef &sstd);

void to_json(json &j, const std::unique_ptr<ScalarSerializableTypeDef> &sstd_ptr);

void from_json(const json &j, ScalarSerializableTypeDef &sstd);

struct MessageSerializableTypeDef : public SerializableTypeDef {
  MessageSerializableTypeDef() = default;
  MessageSerializableTypeDef(std::string nm, bool opt): name(nm), SerializableTypeDef(opt) {}
  [[nodiscard]] TypeDef::Type t() const override {return til::TypeDef::kMessage;}
  std::string name{};
};

void to_json(json &j, const MessageSerializableTypeDef &mstd);

void to_json(json &j, const std::unique_ptr<MessageSerializableTypeDef> &mstd_ptr);

void from_json(const json &j, MessageSerializableTypeDef &mstd);

struct ListSerializableTypeDef : public SerializableTypeDef {
  ListSerializableTypeDef() = default;
  ListSerializableTypeDef(std::unique_ptr<SerializableTypeDef> sub_t, bool opt)
      : sub_type(std::move(sub_t)), SerializableTypeDef(opt) {}
  [[nodiscard]] TypeDef::Type t() const override { return TypeDef::kList; }
  std::unique_ptr<SerializableTypeDef> sub_type;
};

void to_json(json &j, const ListSerializableTypeDef &lstd);

void to_json(json &j, const std::unique_ptr<ListSerializableTypeDef> &lstd_ptr);

void from_json(const json &j, ListSerializableTypeDef &lstd);

struct MapSerializableTypeDef : public SerializableTypeDef {
  MapSerializableTypeDef() = default;
  MapSerializableTypeDef(std::unique_ptr<SerializableTypeDef> sub_t, bool opt) : sub_type(std::move(sub_t)),
                                                                                 SerializableTypeDef(opt) {}
  [[nodiscard]] TypeDef::Type t() const override {
    return TypeDef::kMap;
  }
  std::unique_ptr<SerializableTypeDef> sub_type;
};

void to_json(json &j, const MapSerializableTypeDef &mstd);

void to_json(json &j, const std::unique_ptr<MapSerializableTypeDef> &mstd_ptr);

void from_json(const json &j, MapSerializableTypeDef &mstd);
}

#endif //TELL_COMPILER_SERIALIZABLETYPEDEF_H_
