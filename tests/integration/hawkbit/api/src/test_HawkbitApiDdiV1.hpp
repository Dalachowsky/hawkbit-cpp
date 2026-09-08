#include <gtest/gtest.h>
#include <iostream>
#include <string>

#include <hawkbit/api/HawkbitApiDdiV1.hpp>

using namespace hawkbit;
using namespace hawkbit::api;

class HawkbitTestContainer : public ::testing::Test {

protected:
  void SetUp() override {};

  void TearDown() override {};

  void debugResponse(const ddi::DdiResponse &res) {
    fmt::println("\n----------[RESPONSE]---------");
    if (std::holds_alternative<dto::ExceptionInfo>(res.data)) {
      auto e = std::get<dto::ExceptionInfo>(res.data);
      fmt::println("Server returned exception:");
      fmt::println("  Code: {}", e.errorCode.value_or("-"));
      fmt::println("  Class: {}", e.exceptionClass.value_or("-"));
      fmt::println("  Message: {}", e.message.value_or("-"));
    } else {
      fmt::println("Server returned: {}", res.code);
    }
    fmt::println("-----------------------------\n");
  }
};

TEST_F(HawkbitTestContainer, putInstalledBase) {
  ddi::HawkbitDdiV1 api("127.0.0.1", 8080, "test_target");
  api.setTargetToken("2345678DGGDGFTDzztgf");

  auto dto = dto::DdiAssignedVersion{"Test distribution set", "1.0.0"};
  auto result = api.putInstalledBase(dto);
  debugResponse(result);
  ASSERT_EQ(result.status, ddi::ResponseStatus::OK);
  ASSERT_EQ(result.code, 200);
  ASSERT_FALSE(result.holdsData());
}

TEST_F(HawkbitTestContainer, putConfigData) {
  ddi::HawkbitDdiV1 api("127.0.0.1", 8080, "test_target");
  api.setTargetToken("2345678DGGDGFTDzztgf");

  auto dto = dto::DdiConfigData{};
  dto.mode = dto::DdiConfigData::Mode::replace;
  dto.data = {{"hw_rev", "2"}};
  auto result = api.putConfigData(dto);
  debugResponse(result);
  ASSERT_EQ(result.status, ddi::ResponseStatus::OK);
  ASSERT_EQ(result.code, 200);
  ASSERT_FALSE(result.holdsData());
}

TEST_F(HawkbitTestContainer, getRootResource) {
  ddi::HawkbitDdiV1 api("127.0.0.1", 8080, "test_target");
  api.setTargetToken("2345678DGGDGFTDzztgf");
  auto result = api.getRootResource();
  ASSERT_EQ(result.status, ddi::ResponseStatus::OK);
  ASSERT_EQ(result.code, 200);
  ASSERT_TRUE(std::holds_alternative<dto::DdiControllerBase>(result.data));
}

TEST_F(HawkbitTestContainer, notAuthorized) {
  ddi::HawkbitDdiV1 api("127.0.0.1", 8080, "test_targe");
  auto result = api.getRootResource();
  ASSERT_EQ(result.status, ddi::ResponseStatus::ERROR_NOT_AUTHORIZED);
  ASSERT_EQ(result.code, 401);
}
