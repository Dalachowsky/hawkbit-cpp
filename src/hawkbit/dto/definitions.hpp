#pragma once

#include <chrono>
#include <map>
#include <optional>
#include <string>

namespace hawkbit::dto {

using clock = std::chrono::steady_clock;

using Details = std::vector<std::string>;

// EXPERIMENTAL
struct DdiAssignedVersion {
  std::string name;
  std::string version;
};

struct ExceptionInfo {
  std::optional<std::string> exceptionClass;
  std::optional<std::string> errorCode;
  std::optional<std::string> message;
};

struct DdiConfigData {
  enum Mode { merge, replace, remove };

  std::map<std::string, std::string> data; // TODO minimum 1
  std::optional<Mode> mode;                // Defaults to merge
};

struct DdiProgress {
  DdiProgress() {}
  explicit DdiProgress(int cnt) : cnt(cnt) {}
  DdiProgress(int cnt, int of) : cnt(cnt), of(of) {}

  int cnt;
  std::optional<int> of;
};

struct DdiResult {
  enum Finished {
    success,
    failure,
    none,
  };

  Finished finished;
  std::optional<DdiProgress> progress;
};

struct DdiStatus {
  enum Execution {
    closed,
    proceeding,
    canceled,
    scheduled,
    rejected,
    resumed,
    downloaded,
    download,
  };

  Execution execution;
  DdiResult result;
  std::optional<int32_t> code;
  std::optional<Details> details;
};

struct DdiActionFeedback {
  DdiStatus status;
  std::optional<std::chrono::milliseconds> timestamp;
};

struct DdiConfirmationFeedback {
  enum Confirmation {
    confirmed,
    denied,
  };
  Confirmation confirmation;
  std::optional<int32_t> code;
  std::optional<Details> details;
};

struct DdiActivateAutoConfirmation {
  std::optional<std::string> initiator;
  std::optional<std::string> remark;
};

struct DdiPolling {
  std::optional<std::chrono::seconds> sleep;
};

struct DdiConfig {
  std::optional<DdiPolling> polling;
};

struct Link {
  std::optional<std::string> href;
};

using Links = std::map<std::string, Link>;

struct DdiControllerBase {
  std::optional<DdiConfig> config;
  std::optional<Links> links;
};

struct DdiArtifactHash {
  std::optional<std::string> sha1;
  std::optional<std::string> md5;
  std::optional<std::string> sha256;
};

struct DdiArtifact {
  std::string filename;
  std::optional<DdiArtifactHash> hashes;
  std::optional<int64_t> size;
  std::optional<Links> links;
};

// Used in
// /{tenant}/controller/v1/{controllerId}/softwaremodules/{softwareModuleId}/artifacts
using DdiArtifactList = std::vector<DdiArtifact>;

struct DdiActionHistory {
  std::optional<std::string> status;
  std::optional<std::vector<std::string>> messages;
};

struct DdiMetadataEntry {
  std::string key;
  std::string value;
};
using DdiMetadata = std::vector<DdiMetadataEntry>;

struct DdiChunk {
  std::string part;
  std::string version;
  std::string name;
  std::optional<bool> encrypted;
  std::optional<std::vector<DdiArtifact>> artifacts;
  std::optional<DdiMetadata> metadata;
};

struct DdiDeployment {
  enum _Handling {
    skip,
    attempt,
    forced,
  };
  using Download = _Handling;
  using Update = _Handling;
  enum MaintenanceWindow {
    available,
    unavailable,
  };

  std::optional<Download> download;
  std::optional<Update> update;
  std::vector<DdiChunk> chunks;
  std::optional<MaintenanceWindow> maintenanceWindow;
};

struct DdiDeploymentBase {
  std::string id;
  DdiDeployment deployment;
  std::optional<DdiActionHistory> actionHistory;
  std::optional<Links> links;
};

struct DdiAutoConfirmationState {
  bool active;
  std::optional<std::string> initiator;
  std::optional<std::string> remark;
  std::optional<int64_t> activatedAt;
  std::optional<Links> links;
};

struct DdiConfirmationBase {
  DdiAutoConfirmationState autoConfirm;
  std::optional<Links> links;
};

struct DdiConfirmationBaseAction {
  std::string id;
  DdiDeployment confirmation;
  std::optional<DdiActionHistory> actionHistory;
  std::optional<Links> links;
};

struct DdiCancelActionToStop {
  std::string stopId;
};

struct DdiCancel {
  std::optional<std::string> id;
  DdiCancelActionToStop cancelAction;
};

} // namespace hawkbit::dto