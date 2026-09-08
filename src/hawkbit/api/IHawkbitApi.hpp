#pragma once

#include <filesystem>
#include <ostream>
#include <variant>

#include "hawkbit/dto/definitions.hpp"

namespace hawkbit::api::ddi {

namespace fs = std::filesystem;

enum ResponseStatus {
  OK,
  ERROR,
  ERROR_MALFORMED_RESPONSE,
  ERROR_NOT_AUTHORIZED
};

using ResponseData =
    std::variant<std::monostate, dto::DdiAssignedVersion, dto::DdiConfigData,
                 dto::DdiActionFeedback, dto::DdiConfirmationFeedback,
                 dto::DdiActivateAutoConfirmation, dto::DdiControllerBase,
                 dto::DdiArtifact, dto::DdiDeploymentBase,
                 dto::DdiConfirmationBase, dto::DdiConfirmationBaseAction,
                 dto::DdiCancel, dto::ExceptionInfo>;

struct DdiResponse {
  ResponseStatus status;
  int code;
  ResponseData data{std::monostate{}};

  bool holdsData() const {
    return !std::holds_alternative<std::monostate>(data);
  }
};

class IHawkbitDdiV1 {
public:
  // /{tenant}/controller/v1/{controllerId}/installedBase
  virtual DdiResponse putInstalledBase(const dto::DdiAssignedVersion &dto) = 0;
  // /{tenant}/controller/v1/{controllerId}/configData
  virtual DdiResponse putConfigData(const dto::DdiConfigData &dto) = 0;

  // /{tenant}/controller/v1/{controllerId}/deploymentBase/{actionId}/feedback
  virtual DdiResponse
  postDeploymentBaseFeedback(const dto::DdiActionFeedback &dto,
                             const int64_t actionId) = 0;
  // /{tenant}/controller/v1/{controllerId}/confirmationBase/{actionId}/feedback
  virtual DdiResponse
  postConfirmationBaseFeedback(const dto::DdiConfirmationFeedback &dto,
                               const int64_t actionId) = 0;
  // /{tenant}/controller/v1/{controllerId}/confirmationBase/deactivateAutoConfirm
  virtual DdiResponse postDeactivateAutoConfirm() = 0;
  // /{tenant}/controller/v1/{controllerId}/confirmationBase/activateAutoConfirm
  virtual DdiResponse
  postActivateAutoConfirm(const dto::DdiActivateAutoConfirmation &dto) = 0;
  // /{tenant}/controller/v1/{controllerId}/cancelAction/{actionId}/feedback
  virtual DdiResponse
  postCancelActionFeedback(const dto::DdiActionFeedback &dto,
                           const int64_t actionId) = 0;

  // /{tenant}/controller/v1/{controllerId}
  virtual DdiResponse getRootResource() = 0;
  // /{tenant}/controller/v1/{controllerId}/softwaremodules/{softwareModuleId}/artifacts
  virtual DdiResponse
  getSoftwareModuleArtifacts(const int64_t softwareModuleId) = 0;
  // /{tenant}/controller/v1/{controllerId}/softwaremodules/{softwareModuleId}/artifacts/{fileName}
  virtual DdiResponse getSoftwareModuleArtifact(const int64_t softwareModuleId,
                                                const std::string &fileName,
                                                std::ostream &ostr) = 0;
  // /{tenant}/controller/v1/{controllerId}/softwaremodules/{softwareModuleId}/artifacts/{fileName}.MD5SUM
  virtual DdiResponse
  getSoftwareModuleArtifactMd5Sum(const int64_t softwareModuleId,
                                  const std::string &fileName,
                                  std::ostream &ostr) = 0;
  // /{tenant}/controller/v1/{controllerId}/installedBase/{actionId}
  virtual DdiResponse getInstalledBase(const int64_t actionId,
                                       const int64_t actionHistory = 0) = 0;
  // /{tenant}/controller/v1/{controllerId}/deploymentBase/{actionId}
  virtual DdiResponse getDeplymentBase(const int64_t actionId,
                                       const int64_t actionHistory = 0) = 0;
  // /{tenant}/controller/v1/{controllerId}/confirmationBase
  virtual DdiResponse getConfirmationBase() = 0;
  // /{tenant}/controller/v1/{controllerId}/confirmationBase/{actionId}
  virtual DdiResponse getConfirmationBase(const int64_t actionId,
                                          const int64_t actionHistory = 0) = 0;
  // /{tenant}/controller/v1/{controllerId}/cancelAction/{actionId}
  virtual DdiResponse getCancelAction(const int64_t actionId) = 0;
};

} // namespace hawkbit::api::ddi
