#pragma once

#include <fmt/format.h>
#include <nlohmann/json.hpp>
#include <string>

namespace nl = nlohmann;

namespace hawkbit::dto {

class ParseError : std::runtime_error {
public:
  ParseError(const std::string &jsonRaw, const std::exception &e)
      : std::runtime_error(e.what()), json(jsonRaw) {}
  const std::string json;
};

template <typename dtoType> class SerializeError : std::runtime_error {
public:
  SerializeError(const std::exception &e)
      : std::runtime_error(fmt::format("Error serializing {}. {}",
                                       typeid(dtoType).name(), e.what())) {}
};

template <typename dtoType> dtoType fromJSON(const nl::json &);
template <typename dtoType> nl::json toJSON(const dtoType &);

template <typename dtoType> dtoType parse(const std::string &s) {
  try {
    auto j = nl::json::parse(s);
    return fromJSON<dtoType>(j);
  } catch (const std::exception &e) {
    throw ParseError(s, e);
  }
}

template <typename dtoType> std::string serialize(const dtoType &dto) {
  try {
    nl::json j = toJSON<dtoType>(dto);
    return j.dump();
  } catch (const std::exception &e) {
    throw SerializeError<dtoType>(e);
  }
}

}; // namespace hawkbit::dto