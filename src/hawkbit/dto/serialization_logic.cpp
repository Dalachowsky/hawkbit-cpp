
#include "common.hpp"
#include "definitions.hpp"

#include <fmt/chrono.h>

namespace hawkbit::dto {

template <> nl::json toJSON<Details>(const Details &d) {
  nl::json j = d;
  return j;
}

template <> nl::json toJSON<Link>(const Link &d) {
  nl::json j;

  if (d.href.has_value()) {
    j["href"] = d.href.value();
  }

  return j;
}

template <> nl::json toJSON<Links>(const Links &d) {
  nl::json j;

  for (const auto &[name, link] : d) {
    j[name] = toJSON(link);
  }

  return j;
}

template <> nl::json toJSON<DdiAssignedVersion>(const DdiAssignedVersion &d) {
  nl::json j;
  j["name"] = d.name;
  j["version"] = d.version;
  return j;
}

template <> nl::json toJSON<ExceptionInfo>(const ExceptionInfo &d) {
  nl::json j;
  if (d.errorCode.has_value()) {
    j["errorCode"] = d.errorCode.value();
  }
  if (d.exceptionClass.has_value()) {
    j["exceptionClass"] = d.exceptionClass.value();
  }
  if (d.message.has_value()) {
    j["message"] = d.message.value();
  }
  return j;
}

template <> nl::json toJSON<DdiConfigData>(const DdiConfigData &d) {
  nl::json j;

  if (d.mode.has_value()) {
    static const std::map<DdiConfigData::Mode, std::string>
        modeSerializationMap = {
            {DdiConfigData::Mode::merge, "merge"},
            {DdiConfigData::Mode::replace, "replace"},
            {DdiConfigData::Mode::remove, "remove"},
        };
    j["mode"] = modeSerializationMap.at(d.mode.value());
  }
  if (d.data.size() < 1) {
    throw std::runtime_error("DdiConfigData: data field cannot be empty");
  }
  j["data"] = d.data;

  return j;
}

template <> nl::json toJSON<DdiProgress>(const DdiProgress &d) {
  nl::json j;

  j["cnt"] = d.cnt;
  if (d.of.has_value()) {
    j["of"] = d.of.value();
  }

  return j;
}

template <> nl::json toJSON<DdiResult>(const DdiResult &d) {
  nl::json j;

  const std::map<DdiResult::Finished, std::string> finishedSerializationMap = {
      {DdiResult::Finished::success, "success"},
      {DdiResult::Finished::failure, "failure"},
      {DdiResult::Finished::none, "none"},
  };
  j["finished"] = finishedSerializationMap.at(d.finished);
  if (d.progress.has_value()) {
    j["progress"] = toJSON(d.progress.value());
  }

  return j;
}

template <> nl::json toJSON<DdiStatus>(const DdiStatus &d) {
  nl::json j;

  const std::map<DdiStatus::Execution, std::string> executionSerializeMap = {
      {DdiStatus::Execution::closed, "closed"},
      {DdiStatus::Execution::proceeding, "proceeding"},
      {DdiStatus::Execution::canceled, "canceled"},
      {DdiStatus::Execution::scheduled, "scheduled"},
      {DdiStatus::Execution::rejected, "rejected"},
      {DdiStatus::Execution::resumed, "resumed"},
      {DdiStatus::Execution::downloaded, "downloaded"},
      {DdiStatus::Execution::download, "download"},
  };
  j["execution"] = executionSerializeMap.at(d.execution);
  j["result"] = toJSON(d.result);
  if (d.code.has_value()) {
    j["code"] = d.code.value();
  }
  if (d.details.has_value()) {
    j["details"] = toJSON(d.details.value());
  }

  return j;
}

template <> nl::json toJSON<DdiActionFeedback>(const DdiActionFeedback &d) {
  nl::json j;

  j["status"] = toJSON<DdiStatus>(d.status);
  if (d.timestamp.has_value()) {
    j["timestamp"] = d.timestamp.value().count();
  }

  return j;
}

template <>
nl::json toJSON<DdiConfirmationFeedback>(const DdiConfirmationFeedback &d) {
  nl::json j;

  static const std::map<DdiConfirmationFeedback::Confirmation, std::string>
      confirmationSerializationMap = {
          {DdiConfirmationFeedback::Confirmation::confirmed, "confirmed"},
          {DdiConfirmationFeedback::Confirmation::denied, "denied"},
      };
  j["confirmation"] = confirmationSerializationMap.at(d.confirmation);
  if (d.code.has_value()) {
    j["code"] = d.code.value();
  }
  if (d.details.has_value()) {
    j["details"] = toJSON(d.details.value());
  }

  return j;
}

template <>
nl::json
toJSON<DdiActivateAutoConfirmation>(const DdiActivateAutoConfirmation &d) {
  nl::json j;

  if (d.initiator.has_value()) {
    j["initiator"] = d.initiator.value();
  }
  if (d.remark.has_value()) {
    j["remark"] = d.remark.value();
  }

  return j;
}

template <> nl::json toJSON<DdiArtifactHash>(const DdiArtifactHash &d) {
  nl::json j;

  if (d.md5.has_value()) {
    j["md5"] = d.md5.value();
  }
  if (d.sha1.has_value()) {
    j["sha1"] = d.sha1.value();
  }
  if (d.sha256.has_value()) {
    j["sha256"] = d.sha256.value();
  }

  return j;
}

template <> nl::json toJSON<DdiArtifact>(const DdiArtifact &d) {
  nl::json j;

  j["filename"] = d.filename;
  if (d.size.has_value()) {
    j["size"] = d.size.value();
  }
  if (d.hashes.has_value()) {
    j["hashes"] = toJSON(d.hashes.value());
  }
  if (d.links.has_value()) {
    j["_links"] = toJSON(d.links.value());
  }

  return j;
}

template <> nl::json toJSON<DdiArtifactList>(const DdiArtifactList &d) {
  nl::json j;

  for (const auto &artifact : d) {
    j.push_back(toJSON(artifact));
  }

  return j;
}

template <> nl::json toJSON<DdiActionHistory>(const DdiActionHistory &d) {
  nl::json j;

  if (d.status.has_value()) {
    j["status"] = d.status.value();
  }
  if (d.messages.has_value()) {
    j["messages"] = d.messages.value();
  }

  return j;
}

template <> nl::json toJSON<DdiMetadataEntry>(const DdiMetadataEntry &d) {
  nl::json j = {
      {"key", d.key},
      {"value", d.value},
  };
  return j;
}

template <> nl::json toJSON<DdiMetadata>(const DdiMetadata &d) {
  nl::json j;

  for (const auto &entry : d) {
    j.push_back(toJSON(entry));
  }

  return j;
}

template <> nl::json toJSON<DdiChunk>(const DdiChunk &d) {
  nl::json j;

  j["part"] = d.part;
  j["version"] = d.version;
  j["name"] = d.name;
  if (d.encrypted.has_value()) {
    j["encrypted"] = d.encrypted.value();
  }
  if (d.artifacts.has_value()) {
    j["artifacts"] = toJSON(d.artifacts.value());
  }
  if (d.metadata.has_value()) {
    j["metadata"] = toJSON(d.metadata.value());
  }

  return j;
}

template <> nl::json toJSON<DdiDeployment>(const DdiDeployment &d) {
  nl::json j;

  static const std::map<DdiDeployment::_Handling, std::string>
      handlingSerializationMap = {
          {DdiDeployment::_Handling::skip, "skip"},
          {DdiDeployment::_Handling::attempt, "attempt"},
          {DdiDeployment::_Handling::forced, "forced"},
      };
  static const std::map<DdiDeployment::MaintenanceWindow, std::string>
      maintenanceWindowSerializationMap = {
          {DdiDeployment::MaintenanceWindow::available, "available"},
          {DdiDeployment::MaintenanceWindow::unavailable, "unavailable"},
      };
  if (d.download.has_value()) {
    j["download"] = handlingSerializationMap.at(d.download.value());
  }
  if (d.update.has_value()) {
    j["update"] = handlingSerializationMap.at(d.update.value());
  }
  if (d.maintenanceWindow.has_value()) {
    j["maintenanceWindow"] =
        maintenanceWindowSerializationMap.at(d.maintenanceWindow.value());
  }

  for (const auto &chunk : d.chunks) {
    j["chunks"].push_back(toJSON(chunk));
  }

  return j;
}

template <> nl::json toJSON<DdiDeploymentBase>(const DdiDeploymentBase &d) {
  nl::json j;

  j["id"] = d.id;
  j["deployment"] = toJSON(d.deployment);
  if (d.actionHistory.has_value()) {
    j["actionHistory"] = toJSON(d.actionHistory.value());
  }
  if (d.links.has_value()) {
    j["_links"] = toJSON(d.links.value());
  }

  return j;
}

template <>
nl::json toJSON<DdiAutoConfirmationState>(const DdiAutoConfirmationState &d) {
  nl::json j;

  j["active"] = d.active;
  if (d.activatedAt.has_value()) {
    j["activatedAt"] = d.activatedAt.value();
  }
  if (d.initiator.has_value()) {
    j["initiator"] = d.initiator.value();
  }
  if (d.remark.has_value()) {
    j["remark"] = d.remark.value();
  }
  if (d.links.has_value()) {
    j["_links"] = toJSON(d.links.value());
  }

  return j;
}

template <> nl::json toJSON<DdiConfirmationBase>(const DdiConfirmationBase &d) {
  nl::json j;

  j["autoConfirm"] = toJSON(d.autoConfirm);
  if (d.links.has_value()) {
    j["_links"] = toJSON(d.links.value());
  }

  return j;
}

template <>
nl::json toJSON<DdiConfirmationBaseAction>(const DdiConfirmationBaseAction &d) {
  nl::json j;

  j["id"] = d.id;
  j["confirmation"] = toJSON(d.confirmation);
  if (d.actionHistory.has_value()) {
    j["actionHistory"] = toJSON(d.actionHistory.value());
  }
  if (d.links.has_value()) {
    j["_links"] = toJSON(d.links.value());
  }

  return j;
}

template <>
nl::json toJSON<DdiCancelActionToStop>(const DdiCancelActionToStop &d) {
  nl::json j;

  j["stopId"] = d.stopId;

  return j;
}

template <> nl::json toJSON<DdiCancel>(const DdiCancel &d) {
  nl::json j;

  if (d.id.has_value()) {
    j["id"] = d.id.value();
  }
  j["cancelAction"] = toJSON(d.cancelAction);

  return j;
}

template <> nl::json toJSON<DdiPolling>(const DdiPolling &d) {
  nl::json j;

  if (d.sleep.has_value()) {
    j["sleep"] = fmt::format("{:%H:%M:%S}", d.sleep.value());
  }

  return j;
}

template <> nl::json toJSON<DdiConfig>(const DdiConfig &d) {
  nl::json j;

  if (d.polling.has_value()) {
    j["polling"] = toJSON(d.polling.value());
  }

  return j;
}

template <> nl::json toJSON<DdiControllerBase>(const DdiControllerBase &d) {
  nl::json j;

  if (d.config.has_value()) {
    j["config"] = toJSON(d.config.value());
  }
  if (d.links.has_value()) {
    j["_links"] = toJSON(d.links.value());
  }

  return j;
}

} // namespace hawkbit::dto