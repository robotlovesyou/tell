#include "SerializableTypeDef.h"

static const char *K_SUB_TYPE = "sub_type";
static const char *K_T = "t";
static const char *K_SCALAR_TYPE = "scalar_type";
static const char *K_IS_OPTIONAL = "is_optional";
static const char *K_NAME = "name";

void til::to_json(json &j, const til::SerializableTypeDef *td) {
  switch (td->t()) {
    case til::TypeDef::kScalar: {
      const auto *st = dynamic_cast<const til::ScalarSerializableTypeDef *>(td);
      j = *st;
      break;
    }
    case til::TypeDef::kMessage: {
      const auto *mt = dynamic_cast<const til::MessageSerializableTypeDef *>(td);
      j = *mt;
      break;
    }
    case til::TypeDef::kMap: {
      const auto *mt = dynamic_cast<const til::MapSerializableTypeDef *>(td);
      j = *mt;
      break;
    }
    case til::TypeDef::kList: {
      const auto *lt = dynamic_cast<const til::ListSerializableTypeDef *>(td);
      j = *lt;
      break;
    }
  }
}

void til::from_json(const json &j, std::unique_ptr<SerializableTypeDef> &td) {
  auto sub_t = j.at(K_T).get<til::TypeDef::Type>();

  switch (sub_t) {
    case til::TypeDef::kScalar: {
      auto sstd = j.get<til::ScalarSerializableTypeDef>();
      auto sub_type = std::make_unique<til::ScalarSerializableTypeDef>();
      *sub_type = std::move(sstd);
      td = std::move(sub_type);
      break;
    }
    case til::TypeDef::kMap: {
      auto map_std = j.get<til::MapSerializableTypeDef>();
      auto sub_type = std::make_unique<til::MapSerializableTypeDef>();
      *sub_type = std::move(map_std);
      td = std::move(sub_type);
      break;
    }
    case til::TypeDef::kList:{
      auto list_std = j.get<til::ListSerializableTypeDef>();
      auto sub_type = std::make_unique<til::ListSerializableTypeDef>();
      *sub_type = std::move(list_std);
      td = std::move(sub_type);
      break;
    }
    case til::TypeDef::kMessage:{
      auto msg_std = j.get<til::MessageSerializableTypeDef>();
      auto sub_type = std::make_unique<til::MessageSerializableTypeDef>();
      *sub_type = std::move(msg_std);
      td = std::move(sub_type);
      break;
    }
  }
}

void til::to_json(json &j, const ScalarSerializableTypeDef &sstd) {
  j = json{{K_T, sstd.t()}, {K_SCALAR_TYPE, sstd.scalar_type}, {K_IS_OPTIONAL, sstd.is_optional}};
}

void til::to_json(json &j, const std::unique_ptr<ScalarSerializableTypeDef> &sstd_ptr) {
  to_json(j, *sstd_ptr);
}

void til::from_json(const json &j, ScalarSerializableTypeDef &sstd) {
  j.at(K_SCALAR_TYPE).get_to(sstd.scalar_type);
  j.at(K_IS_OPTIONAL).get_to(sstd.is_optional);
}

void til::to_json(json &j, const MessageSerializableTypeDef &mstd) {
  j = json{{K_T, mstd.t()}, {K_NAME, mstd.name}, {K_IS_OPTIONAL, mstd.is_optional}};
}

void til::to_json(json &j, const std::unique_ptr<MessageSerializableTypeDef> &mstd_ptr) {
  to_json(j, *mstd_ptr);
}

void til::from_json(const json &j, MessageSerializableTypeDef &mstd) {
  j.at(K_NAME).get_to(mstd.name);
  j.at(K_IS_OPTIONAL).get_to(mstd.is_optional);
}

void til::to_json(json &j, const MapSerializableTypeDef &mstd) {
  j[K_T] = mstd.t();
  j[K_IS_OPTIONAL] = mstd.is_optional;
  j[K_SUB_TYPE] = mstd.sub_type.get();
}

void til::to_json(json &j, const std::unique_ptr<MapSerializableTypeDef> &mstd_ptr) {
  to_json(j, *mstd_ptr);
}

void til::from_json(const json &j, MapSerializableTypeDef &mstd) {
  j.at(K_IS_OPTIONAL).get_to(mstd.is_optional);
  mstd.sub_type = std::move(j.at(K_SUB_TYPE).get<std::unique_ptr<SerializableTypeDef>>());
}

void til::to_json(json &j, const til::ListSerializableTypeDef &lstd) {
  j[K_T] = lstd.t();
  j[K_IS_OPTIONAL] = lstd.is_optional;
  j[K_SUB_TYPE] = lstd.sub_type.get();
}

void til::to_json(json &j, const std::unique_ptr<ListSerializableTypeDef> &lstd_ptr) {
  to_json(j, *lstd_ptr);
}

void til::from_json(const json &j, til::ListSerializableTypeDef &lstd) {
  j.at(K_IS_OPTIONAL).get_to(lstd.is_optional);
  lstd.sub_type = std::move(j.at(K_SUB_TYPE).get<std::unique_ptr<SerializableTypeDef>>());
}

