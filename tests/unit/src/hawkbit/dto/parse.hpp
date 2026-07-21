
#include <gtest/gtest.h>

#include <hawkbit/dto/common.hpp>
#include <hawkbit/dto/definitions.hpp>

using namespace hawkbit::dto;

TEST(Parse, throw_parse_error_on_exception) {
  std::string raw = "THIS_IS_NOT_A_VALID_JSON";
  ASSERT_THROW({ parse<DdiControllerBase>(raw); }, ParseError);
}
