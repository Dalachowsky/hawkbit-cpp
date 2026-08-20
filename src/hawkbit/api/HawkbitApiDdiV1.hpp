#pragma once

#include <optional>

#include "HttpClient.hpp"
#include "IHawkbitApi.hpp"
#include <hawkbit/dto/common.hpp>

#include <Poco/Net/HTTPClientSession.h>

namespace hawkbit::api::ddi {

class HawkbitDdiV1 : public IHawkbitDdiV1 {
public:
  HawkbitDdiV1(const std::string &host, unsigned int port,
               const std::string &controllerId);

  void setTargetToken(const std::string &token);

  DdiResponse putInstalledBase(const dto::DdiAssignedVersion &dto) override;
  DdiResponse putConfigData(const dto::DdiConfigData &dto) override;

  DdiResponse postDeploymentBaseFeedback(const dto::DdiActionFeedback &dto,
                                         const int64_t actionId) override {
    return {};
  }
  DdiResponse
  postConfirmationBaseFeedback(const dto::DdiConfirmationFeedback &dto,
                               const int64_t actionId) override {
    return {};
  }
  DdiResponse postDeactivateAutoConfirm() override { return {}; }
  DdiResponse postActivateAutoConfirm(
      const dto::DdiActivateAutoConfirmation &dto) override {
    return {};
  }
  DdiResponse postCancelActionFeedback(const dto::DdiActionFeedback &dto,
                                       const int64_t actionId) override {
    return {};
  }

  DdiResponse getRootResource() override;
  DdiResponse
  getSoftwareModuleArtifacts(const int64_t softwareModuleId) override {
    return {};
  }
  DdiResponse getSoftwareModuleArtifact(const int64_t softwareModuleId,
                                        const std::string &fileName,
                                        std::ostream &ostr) override {
    return {};
  }
  DdiResponse getSoftwareModuleArtifactMd5Sum(const int64_t softwareModuleId,
                                              const std::string &fileName,
                                              std::ostream &ostr) override {
    return {};
  }
  DdiResponse getInstalledBase(const int64_t actionId,
                               const int64_t actionHistory = 0) override {
    return {};
  }
  DdiResponse getDeplymentBase(const int64_t actionId,
                               const int64_t actionHistory = 0) override {
    return {};
  }
  DdiResponse getConfirmationBase() override { return {}; }
  DdiResponse getConfirmationBase(const int64_t actionId,
                                  const int64_t actionHistory = 0) override {
    return {};
  }
  DdiResponse getCancelAction(const int64_t actionId) override { return {}; }

private:
  static const std::string TENANT;
  const std::string serverURL;
  const std::string controllerId;

  HttpClient client;

  DdiResponse returnExceptionInfo(const HttpResponse &res);
  template <typename DtoType>
  DdiResponse returnParsedDTO(const HttpResponse &res) {
    DdiResponse ret{ResponseStatus::OK, res.code};
    try {
      ret.data = dto::parse<DtoType>(std::get<std::string>(res.data));
    } catch (const std::exception &e) {
      // TODO log error
      ret.status = ResponseStatus::ERROR_MALFORMED_RESPONSE;
    }
    return ret;
  }
};

} // namespace hawkbit::api::ddi