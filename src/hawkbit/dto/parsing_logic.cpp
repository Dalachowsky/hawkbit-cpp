
#include "common.hpp"
#include "definitions.hpp"

#include <regex>

#define GET_OPTIONAL(dst, json, key)                                           \
  if (j.contains(key)) {                                                       \
    dst = json.at(key);                                                        \
  }

#define THROW_IF_ELEMENT_NOT_FOUND(dtoName, json, key)                         \
  if (!json.contains(key)) {                                                   \
    throw std::runtime_error(dtoName " does not contain " key);                \
  }

#define THROW_IF_ELEMENT_IS_WRONG_TYPE(dtoName, json, key, nlohmannJsonType)   \
  if (!json.at(key).is_##nlohmannJsonType()) {                                 \
    throw std::runtime_error(dtoName ": element " key " has wrong type");      \
  }

namespace hawkbit::dto {

using clock = std::chrono::steady_clock;

template <> Details fromJSON<Details>(const nl::json &j) {
  Details dto;

  if (!j.is_array()) {
    throw std::runtime_error("Details are not array");
  }

  for (auto &s : j) {
    dto.push_back(s);
  }

  return dto;
}

template <> Link fromJSON<Link>(const nl::json &j) {
  Link link;

  if (j.contains("href")) {
    link.href = j.at("href");
  }

  return link;
}

template <> Links fromJSON<Links>(const nl::json &j) {
  Links links;
  if (!j.is_object()) {
    throw std::runtime_error("Links is not an object");
  }
  for (auto el : j.items()) {
    links[el.key()] = fromJSON<Link>(el.value());
  }

  return links;
}

template <> DdiAssignedVersion fromJSON<DdiAssignedVersion>(const nl::json &j) {
  DdiAssignedVersion dto;

  THROW_IF_ELEMENT_NOT_FOUND("DdiAssignedVersion", j, "name");
  THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiAssignedVersion", j, "name", string);
  THROW_IF_ELEMENT_NOT_FOUND("DdiAssignedVersion", j, "version");
  THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiAssignedVersion", j, "version", string);

  dto.name = j.at("name");
  dto.version = j.at("version");

  return dto;
}

template <> ExceptionInfo fromJSON<ExceptionInfo>(const nl::json &j) {
  ExceptionInfo dto;

  GET_OPTIONAL(dto.exceptionClass, j, "exceptionClass");
  GET_OPTIONAL(dto.errorCode, j, "errorCode");
  GET_OPTIONAL(dto.message, j, "message");

  return dto;
}

template <> DdiConfigData fromJSON<DdiConfigData>(const nl::json &j) {
  DdiConfigData dto;

  if (j.contains("mode")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiConfigData", j, "mode", string);
    const std::string mode = j.at("mode");
    static const std::map<std::string, DdiConfigData::Mode> modeParsingMap = {
        {"merge", DdiConfigData::Mode::merge},
        {"replace", DdiConfigData::Mode::replace},
        {"remove", DdiConfigData::Mode::remove},
    };
    if (modeParsingMap.find(mode) == modeParsingMap.end()) {
      throw std::runtime_error(
          fmt::format("Unknown DdiConfigData::Mode {}", mode));
    }
    dto.mode = modeParsingMap.at(mode);
  }

  THROW_IF_ELEMENT_NOT_FOUND("DdiConfigData", j, "data");
  THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiConfigData", j, "data", object);

  for (auto el : j.at("data").items()) {
    dto.data[el.key()] = el.value();
  }

  return dto;
}

template <> DdiProgress fromJSON<DdiProgress>(const nl::json &j) {
  DdiProgress dto;

  THROW_IF_ELEMENT_NOT_FOUND("DdiProgress", j, "cnt");
  THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiProgress", j, "cnt", number);
  dto.cnt = j.at("cnt");
  if (j.contains("of")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiProgress", j, "of", number);
    dto.of = j.at("of");
  }

  return dto;
}

template <> DdiResult fromJSON<DdiResult>(const nl::json &j) {
  DdiResult dto;

  THROW_IF_ELEMENT_NOT_FOUND("DdiResult", j, "finished");
  THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiResult", j, "finished", string);
  const std::string finished = j.at("finished");
  static const std::map<std::string, DdiResult::Finished> finishedParseMap = {
      {"success", DdiResult::Finished::success},
      {"failure", DdiResult::Finished::failure},
      {"none", DdiResult::Finished::none},
  };
  if (finishedParseMap.find(finished) == finishedParseMap.end()) {
    throw std::runtime_error(
        fmt::format("Unknown DdiResult::Finished {}", finished));
  }
  dto.finished = finishedParseMap.at(finished);

  if (j.contains("progress")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiResult", j, "progress", object);
    dto.progress = fromJSON<DdiProgress>(j.at("progress"));
  }

  return dto;
}

template <> DdiStatus fromJSON<DdiStatus>(const nl::json &j) {
  DdiStatus dto;

  static const std::map<std::string, DdiStatus::Execution> executionParsingMap =
      {
          {"closed", DdiStatus::Execution::closed},
          {"proceeding", DdiStatus::Execution::proceeding},
          {"canceled", DdiStatus::Execution::canceled},
          {"scheduled", DdiStatus::Execution::scheduled},
          {"rejected", DdiStatus::Execution::rejected},
          {"resumed", DdiStatus::Execution::resumed},
          {"downloaded", DdiStatus::Execution::downloaded},
          {"download", DdiStatus::Execution::download},
      };

  THROW_IF_ELEMENT_NOT_FOUND("DdiStatus", j, "execution");
  THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiStatus", j, "execution", string);
  const std::string execution = j.at("execution");
  if (executionParsingMap.find(execution) == executionParsingMap.end()) {
    throw std::runtime_error(fmt::format("Unknown execution {}", execution));
  }
  dto.execution = executionParsingMap.at(execution);

  THROW_IF_ELEMENT_NOT_FOUND("DdiStatus", j, "result");
  THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiStatus", j, "result", object);
  dto.result = fromJSON<DdiResult>(j.at("result"));

  if (j.contains("code")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiStatus", j, "code", number);
    dto.code = j.at("code");
  }
  if (j.contains("details")) {
    dto.details = fromJSON<Details>(j.at("details"));
  }

  return dto;
}

template <> DdiActionFeedback fromJSON<DdiActionFeedback>(const nl::json &j) {
  DdiActionFeedback dto;

  THROW_IF_ELEMENT_NOT_FOUND("DdiActionFeedback", j, "status");
  THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiActionFeedback", j, "status", object);

  dto.status = fromJSON<DdiStatus>(j.at("status"));

  if (j.contains("timestamp")) {
    dto.timestamp = std::chrono::milliseconds(j.at("timestamp"));
  }

  return dto;
}

template <>
DdiConfirmationFeedback fromJSON<DdiConfirmationFeedback>(const nl::json &j) {
  DdiConfirmationFeedback dto;

  static const std::map<std::string, DdiConfirmationFeedback::Confirmation>
      confirmationParsingMap = {
          {"confirmed", DdiConfirmationFeedback::Confirmation::confirmed},
          {"denied", DdiConfirmationFeedback::Confirmation::denied},
      };
  THROW_IF_ELEMENT_NOT_FOUND("DdiConfirmationFeedback", j, "confirmation");
  THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiConfirmationFeedback", j, "confirmation",
                                 string);
  const std::string confirmation = j.at("confirmation");
  if (confirmationParsingMap.find(confirmation) ==
      confirmationParsingMap.end()) {
    throw std::runtime_error(
        fmt::format("Unknown confirmation {}", confirmation));
  }
  dto.confirmation = confirmationParsingMap.at(confirmation);

  if (j.contains("code")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiConfirmationFeedback", j, "code",
                                   number);
    dto.code = j.at("code");
  }
  if (j.contains("details")) {
    dto.details = fromJSON<Details>(j.at("details"));
  }

  return dto;
}

template <>
DdiActivateAutoConfirmation
fromJSON<DdiActivateAutoConfirmation>(const nl::json &j) {
  DdiActivateAutoConfirmation dto;

  if (j.contains("initiator")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiActivateAutoConfirmation", j,
                                   "initiator", string);
    dto.initiator = j.at("initiator");
  }
  if (j.contains("remark")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiActivateAutoConfirmation", j, "remark",
                                   string);
    dto.remark = j.at("remark");
  }

  return dto;
}

template <> DdiArtifactHash fromJSON<DdiArtifactHash>(const nl::json &j) {
  DdiArtifactHash dto;

  if (j.contains("sha1")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiArtifactHash", j, "sha1", string);
    dto.sha1 = j.at("sha1");
  }
  if (j.contains("md5")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiArtifactHash", j, "md5", string);
    dto.md5 = j.at("md5");
  }
  if (j.contains("sha256")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiArtifactHash", j, "sha256", string);
    dto.sha256 = j.at("sha256");
  }

  return dto;
}

template <> DdiArtifact fromJSON<DdiArtifact>(const nl::json &j) {
  DdiArtifact dto;

  THROW_IF_ELEMENT_NOT_FOUND("DdiArtifact", j, "filename");
  THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiArtifact", j, "filename", string);
  dto.filename = j.at("filename");

  if (j.contains("hashes")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiArtifact", j, "hashes", object);
    dto.hashes = fromJSON<DdiArtifactHash>(j.at("hashes"));
  }
  if (j.contains("size")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiArtifact", j, "size", number);
    dto.size = j.at("size");
  }
  if (j.contains("_links")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiArtifact", j, "_links", object);
    dto.links = fromJSON<Links>(j.at("_links"));
  }

  return dto;
}

template <> DdiArtifactList fromJSON<DdiArtifactList>(const nl::json &j) {
  DdiArtifactList dto;

  if (!j.is_array()) {
    throw std::runtime_error("DdiArtifactList is not array");
  }

  for (const auto &artifact : j.items()) {
    if (!artifact.value().is_object()) {
      throw std::runtime_error("DdiArtifactList element is not an object");
    }
    dto.push_back(fromJSON<DdiArtifact>(artifact.value()));
  }

  return dto;
}

template <> DdiActionHistory fromJSON<DdiActionHistory>(const nl::json &j) {
  DdiActionHistory dto;

  if (!j.is_object()) {
    throw std::runtime_error("DdiActionHistory is not an object");
  }

  if (j.contains("status")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiActionHistory", j, "status", string);
    dto.status = j.at("status");
  }
  if (j.contains("messages")) {
    dto.messages.emplace();
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiActionHistory", j, "messages", array);
    for (const auto &element : j.at("messages")) {
      if (!element.is_string()) {
        throw std::runtime_error("DdiActionHistory message is not string");
      }
      dto.messages.value().push_back(element);
    }
  }

  return dto;
}

template <> DdiMetadataEntry fromJSON<DdiMetadataEntry>(const nl::json &j) {
  DdiMetadataEntry dto;

  THROW_IF_ELEMENT_NOT_FOUND("DdiMetadataEntry", j, "key");
  THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiMetadataEntry", j, "key", string);
  dto.key = j.at("key");
  THROW_IF_ELEMENT_NOT_FOUND("DdiMetadataEntry", j, "value");
  THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiMetadataEntry", j, "value", string);
  dto.value = j.at("value");

  return dto;
}

template <> DdiMetadata fromJSON<DdiMetadata>(const nl::json &j) {
  DdiMetadata dto;

  if (!j.is_array()) {
    throw std::runtime_error("Metadata is not an array");
  }

  for (const auto &entry : j) {
    dto.push_back(fromJSON<DdiMetadataEntry>(entry));
  }

  return dto;
}

template <> DdiChunk fromJSON<DdiChunk>(const nl::json &j) {
  DdiChunk dto;

  THROW_IF_ELEMENT_NOT_FOUND("DdiChunk", j, "part");
  THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiChunk", j, "part", string);
  dto.part = j.at("part");
  THROW_IF_ELEMENT_NOT_FOUND("DdiChunk", j, "version");
  THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiChunk", j, "version", string);
  dto.version = j.at("version");
  THROW_IF_ELEMENT_NOT_FOUND("DdiChunk", j, "name");
  THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiChunk", j, "name", string);
  dto.name = j.at("name");

  GET_OPTIONAL(dto.encrypted, j, "encrypted");

  if (j.contains("artifacts")) {
    dto.artifacts = fromJSON<DdiArtifactList>(j.at("artifacts"));
  }

  if (j.contains("metadata")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiChunk", j, "metadata", array);
    dto.metadata = fromJSON<DdiMetadata>(j.at("metadata"));
  }

  return dto;
}

template <> DdiDeployment fromJSON<DdiDeployment>(const nl::json &j) {
  DdiDeployment dto;

  if (!j.is_object()) {
    throw std::runtime_error("DdiDeployment is not an object");
  }

  static const std::map<std::string, DdiDeployment::_Handling>
      handlingParsingMap = {
          {"skip", DdiDeployment::_Handling::skip},
          {"attempt", DdiDeployment::_Handling::attempt},
          {"forced", DdiDeployment::_Handling::forced},
      };

  if (j.contains("download")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiDeployment", j, "download", string);
    const std::string download = j.at("download");
    if (handlingParsingMap.find(download) == handlingParsingMap.end()) {
      throw std::runtime_error(fmt::format("Unknown download {}", download));
    }
    dto.download = handlingParsingMap.at(download);
  }
  if (j.contains("update")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiDeployment", j, "update", string);
    const std::string update = j.at("update");
    if (handlingParsingMap.find(update) == handlingParsingMap.end()) {
      throw std::runtime_error(fmt::format("Unknown update {}", update));
    }
    dto.update = handlingParsingMap.at(update);
  }

  THROW_IF_ELEMENT_NOT_FOUND("DdiDeployment", j, "chunks");
  THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiDeployment", j, "chunks", array);
  const auto chunks = j.at("chunks");
  for (const auto &element : chunks) {
    dto.chunks.push_back(fromJSON<DdiChunk>(element));
  }

  if (j.contains("maintenanceWindow")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiDeployment", j, "maintenanceWindow",
                                   string);
    static const std::map<std::string, DdiDeployment::MaintenanceWindow>
        maintenanceWindowParsingMap = {
            {"available", DdiDeployment::MaintenanceWindow::available},
            {"unavailable", DdiDeployment::MaintenanceWindow::unavailable},
        };
    const std::string maintenanceWindow = j.at("maintenanceWindow");
    if (maintenanceWindowParsingMap.find(maintenanceWindow) ==
        maintenanceWindowParsingMap.end()) {
      throw std::runtime_error(
          fmt::format("Unknown maintenanceWindow {}", maintenanceWindow));
    }
    dto.maintenanceWindow = maintenanceWindowParsingMap.at(maintenanceWindow);
  }

  return dto;
}

template <> DdiDeploymentBase fromJSON<DdiDeploymentBase>(const nl::json &j) {
  DdiDeploymentBase dto;

  THROW_IF_ELEMENT_NOT_FOUND("DdiDeploymentBase", j, "id");
  THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiDeploymentBase", j, "id", string);
  dto.id = j.at("id");

  THROW_IF_ELEMENT_NOT_FOUND("DdiDeploymentBase", j, "deployment");
  THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiDeploymentBase", j, "deployment", object);
  dto.deployment = fromJSON<DdiDeployment>(j.at("deployment"));

  if (j.contains("actionHistory")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiDeploymentBase", j, "actionHistory",
                                   object);
    dto.actionHistory = fromJSON<DdiActionHistory>(j.at("actionHistory"));
  }
  if (j.contains("_links")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiDeploymentBase", j, "_links", object);
    dto.links = fromJSON<Links>(j.at("_links"));
  }

  return dto;
}

template <>
DdiAutoConfirmationState fromJSON<DdiAutoConfirmationState>(const nl::json &j) {
  DdiAutoConfirmationState dto;

  THROW_IF_ELEMENT_NOT_FOUND("DdiAutoConfirmationState", j, "active");
  THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiAutoConfirmationState", j, "active",
                                 boolean);
  dto.active = j.at("active");

  if (j.contains("initiator")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiAutoConfirmationState", j, "initiator",
                                   string);
    dto.initiator = j.at("initiator");
  }
  if (j.contains("remark")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiAutoConfirmationState", j, "remark",
                                   string);
    dto.remark = j.at("remark");
  }
  if (j.contains("activatedAt")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiAutoConfirmationState", j, "activatedAt",
                                   number);
    dto.activatedAt = j.at("activatedAt");
  }
  if (j.contains("_links")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiAutoConfirmationState", j, "_links",
                                   object);
    dto.links = fromJSON<Links>(j.at("_links"));
  }

  return dto;
}

template <>
DdiConfirmationBase fromJSON<DdiConfirmationBase>(const nl::json &j) {
  DdiConfirmationBase dto;

  THROW_IF_ELEMENT_NOT_FOUND("DdiConfirmationBase", j, "autoConfirm");
  THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiConfirmationBase", j, "autoConfirm",
                                 object);
  dto.autoConfirm = fromJSON<DdiAutoConfirmationState>(j.at("autoConfirm"));

  if (j.contains("_links")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiConfirmationBase", j, "_links", object);
    dto.links = fromJSON<Links>(j.at("_links"));
  }

  return dto;
}

template <>
DdiConfirmationBaseAction
fromJSON<DdiConfirmationBaseAction>(const nl::json &j) {
  DdiConfirmationBaseAction dto;

  THROW_IF_ELEMENT_NOT_FOUND("DdiConfirmationBaseAction", j, "id");
  THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiConfirmationBaseAction", j, "id", string);
  dto.id = j.at("id");

  THROW_IF_ELEMENT_NOT_FOUND("DdiConfirmationBaseAction", j, "confirmation");
  dto.confirmation = fromJSON<DdiDeployment>(j.at("confirmation"));

  if (j.contains("actionHistory")) {
    dto.actionHistory = fromJSON<DdiActionHistory>(j.at("actionHistory"));
  }
  if (j.contains("_links")) {
    dto.links = fromJSON<Links>(j.at("_links"));
  }

  return dto;
}

template <>
DdiCancelActionToStop fromJSON<DdiCancelActionToStop>(const nl::json &j) {
  DdiCancelActionToStop dto;

  THROW_IF_ELEMENT_NOT_FOUND("DdiCancelActionToStop", j, "stopId");
  THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiCancelActionToStop", j, "stopId", string);
  dto.stopId = j.at("stopId");

  return dto;
}

template <> DdiCancel fromJSON<DdiCancel>(const nl::json &j) {
  DdiCancel dto;

  if (j.contains("id")) {
    THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiCancel", j, "id", string);
    dto.id = j.at("id");
  }

  THROW_IF_ELEMENT_NOT_FOUND("DdiCancel", j, "cancelAction");
  THROW_IF_ELEMENT_IS_WRONG_TYPE("DdiCancel", j, "cancelAction", object);
  dto.cancelAction = fromJSON<DdiCancelActionToStop>(j.at("cancelAction"));

  return dto;
}

template <> DdiPolling fromJSON<DdiPolling>(const nl::json &j) {
  DdiPolling polling;
  auto parseSleep = [&](std::string str) {
    static const std::regex pattern(R"(^([0-9]{2}):([0-9]{2}):([0-9]{2})$)");
    std::smatch match;
    if (!std::regex_match(str, match, pattern))
      throw std::runtime_error(
          fmt::format("Cannot parse sleep: {}. Invalid format", str));

    unsigned hours = std::stoi(match[1].str());
    unsigned minutes = std::stoi(match[2].str());
    unsigned seconds = std::stoi(match[3].str());

    return std::chrono::seconds(hours * 3600 + minutes * 60 + seconds);
  };

  std::string sleep = j.at("sleep");
  polling.sleep = parseSleep(sleep);
  return polling;
}

template <> DdiConfig fromJSON<DdiConfig>(const nl::json &j) {
  DdiConfig config;
  config.polling = fromJSON<DdiPolling>(j.at("polling"));
  return config;
}

template <> DdiControllerBase fromJSON<DdiControllerBase>(const nl::json &j) {
  DdiControllerBase dto;
  if (j.contains("config")) {
    dto.config = fromJSON<DdiConfig>(j.at("config"));
  }
  if (j.contains("_links")) {
    dto.links = fromJSON<Links>(j.at("_links"));
  }
  return dto;
}

} // namespace hawkbit::dto