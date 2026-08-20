#include "HawkbitApiDdiV1.hpp"

#include <hawkbit/dto/common.hpp>

#include <fmt/format.h>

namespace hawkbit::api::ddi {

const std::string HawkbitDdiV1::TENANT = "DEFAULT";

HawkbitDdiV1::HawkbitDdiV1(const std::string &host, unsigned int port,
                           const std::string &controllerId)
    : serverURL(host), controllerId(controllerId), client(host, port) {}

DdiResponse HawkbitDdiV1::putInstalledBase(const dto::DdiAssignedVersion &dto) {
  const std::string uri =
      fmt::format("/{}/controller/v1/{}/installedBase", TENANT, controllerId);
  std::string s = dto::serialize(dto);
  auto res = client.putJson(uri, s);

  DdiResponse ret;
  ret.code = res.code;

  if (res.code == 401) {
    ret.status = ResponseStatus::ERROR_NOT_AUTHORIZED;
    return ret;
  } else if (res.code == 400) {
    return returnExceptionInfo(res);
  } else if (res.code != 200) {
    ret.status = ResponseStatus::ERROR;
    return ret;
  }

  return DdiResponse{ResponseStatus::OK, res.code};
}

DdiResponse HawkbitDdiV1::putConfigData(const dto::DdiConfigData &dto) {}

DdiResponse HawkbitDdiV1::getRootResource() {
  const std::string uri =
      fmt::format("/{}/controller/v1/{}", TENANT, controllerId);

  auto res = client.getString(uri);

  if (res.code == 401) {
    return DdiResponse{ResponseStatus::ERROR_NOT_AUTHORIZED, res.code};
  } else if (res.code == 400) {
    return returnExceptionInfo(res);
  } else if (res.code != 200) {
    return DdiResponse{ResponseStatus::ERROR, res.code};
  }

  return returnParsedDTO<dto::DdiControllerBase>(res);
}

void HawkbitDdiV1::setTargetToken(const std::string &token) {
  client.setCredentials("TargetToken", token);
}

DdiResponse HawkbitDdiV1::returnExceptionInfo(const HttpResponse &res) {
  DdiResponse ret{ResponseStatus::ERROR, res.code};
  try {
    ret.data = dto::parse<dto::ExceptionInfo>(std::get<std::string>(res.data));
  } catch (const std::exception &e) {
    // TODO log error
    ret.status = ResponseStatus::ERROR_MALFORMED_RESPONSE;
  }
  return ret;
}

} // namespace hawkbit::api::ddi
