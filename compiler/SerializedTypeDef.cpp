#include "SerializedTypeDef.h"

static const char *K_SUB_TYPE = "sub_type";
static const char *K_T = "t";
static const char *K_SCALAR_TYPE = "scalar_type";
static const char *K_IS_OPTIONAL = "is_optional";
static const char *K_NAME = "name";

void sub_type_to_json(json &j, const std::unique_ptr<til::SerializedTypeDef> &td_ptr, const char *key) {
  switch (td_ptr->t()) {
    case til::TypeDef::kScalar: {
      const auto *st = dynamic_cast<const til::ScalarSerializedTypeDef *>(td_ptr.get());
      j[key] = *st;
      break;
    }
    case til::TypeDef::kMessage: {
      const auto *mt = dynamic_cast<const til::MessageSerializedTypeDef *>(td_ptr.get());
      j[key] = *mt;
      break;
    }
    case til::TypeDef::kMap: {
      const auto *mt = dynamic_cast<const til::MapSerializedTypeDef *>(td_ptr.get());
      j[key] = *mt;
      break;
    }
    case til::TypeDef::kList:{
      const auto *lt = dynamic_cast<const til::ListSerializedTypeDef *>(td_ptr.get());
      j[key] = *lt;
      break;
    }
  }
}

std::unique_ptr<til::SerializedTypeDef> sub_type_from_json(const json &j, const char *sub_type_key, const char * t_key) {
  auto sub_t = j.at(sub_type_key).at(t_key).get<til::TypeDef::Type>();

  switch (sub_t) {
    case til::TypeDef::kScalar: {
      auto sstd = j.at(K_SUB_TYPE).get<til::ScalarSerializedTypeDef>();
      auto sub_type = std::make_unique<til::ScalarSerializedTypeDef>();
      *sub_type = std::move(sstd);
      return sub_type;
    }
    case til::TypeDef::kMap: {
      auto map_std = j.at(K_SUB_TYPE).get<til::MapSerializedTypeDef>();
      auto sub_type = std::make_unique<til::MapSerializedTypeDef>();
      *sub_type = std::move(map_std);
      return sub_type;
    }
    case til::TypeDef::kList:{
      auto list_std = j.at(K_SUB_TYPE).get<til::ListSerializedTypeDef>();
      auto sub_type = std::make_unique<til::ListSerializedTypeDef>();
      *sub_type = std::move(list_std);
      return sub_type;
    }
    case til::TypeDef::kMessage:{
      auto msg_std = j.at(K_SUB_TYPE).get<til::MessageSerializedTypeDef>();
      auto sub_type = std::make_unique<til::MessageSerializedTypeDef>();
      *sub_type = std::move(msg_std);
      return sub_type;
    }
  }
}

void til::to_json(json &j, const ScalarSerializedTypeDef &sstd) {
  j = json{{K_T, sstd.t()}, {K_SCALAR_TYPE, sstd.scalar_type}, {K_IS_OPTIONAL, sstd.is_optional}};
}

void til::to_json(json &j, const std::unique_ptr<ScalarSerializedTypeDef> &sstd_ptr) {
  to_json(j, *sstd_ptr);
}

void til::from_json(const json &j, ScalarSerializedTypeDef &sstd) {
  j.at(K_SCALAR_TYPE).get_to(sstd.scalar_type);
  j.at(K_IS_OPTIONAL).get_to(sstd.is_optional);
}

void til::to_json(json &j, const MessageSerializedTypeDef &mstd) {
  j = json{{K_T, mstd.t()}, {K_NAME, mstd.name}, {K_IS_OPTIONAL, mstd.is_optional}};
}

void til::to_json(json &j, const std::unique_ptr<MessageSerializedTypeDef> &mstd_ptr) {
  to_json(j, *mstd_ptr);
}

void til::from_json(const json &j, MessageSerializedTypeDef &mstd) {
  j.at(K_NAME).get_to(mstd.name);
  j.at(K_IS_OPTIONAL).get_to(mstd.is_optional);
}

void til::to_json(json &j, const MapSerializedTypeDef &mstd) {
  j[K_T] = mstd.t();
  j[K_IS_OPTIONAL] = mstd.is_optional;
  sub_type_to_json(j, mstd.sub_type, K_SUB_TYPE);
}

void til::to_json(json &j, const std::unique_ptr<MapSerializedTypeDef> &mstd_ptr) {
  to_json(j, *mstd_ptr);
}

void til::from_json(const json &j, MapSerializedTypeDef &mstd) {
  j.at(K_IS_OPTIONAL).get_to(mstd.is_optional);
  mstd.sub_type = std::move(sub_type_from_json(j, K_SUB_TYPE, K_T));
}

void til::to_json(json &j, const til::ListSerializedTypeDef &lstd) {
  j[K_T] = lstd.t();
  j[K_IS_OPTIONAL] = lstd.is_optional;
  sub_type_to_json(j, lstd.sub_type, K_SUB_TYPE);
}

void til::to_json(json &j, const std::unique_ptr<ListSerializedTypeDef> &lstd_ptr) {
  to_json(j, *lstd_ptr);
}

void til::from_json(const json &j, til::ListSerializedTypeDef &lstd) {
  j.at(K_IS_OPTIONAL).get_to(lstd.is_optional);
  lstd.sub_type = std::move(sub_type_from_json(j, K_SUB_TYPE, K_T));
}

