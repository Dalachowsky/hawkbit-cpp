// =====================================================
//
// Tests that parse examples from OpenAPI specification
//
// =====================================================

#include <gtest/gtest.h>

#include <algorithm>

#include <hawkbit/dto/common.hpp>
#include <hawkbit/dto/definitions.hpp>

using namespace hawkbit::dto;

#define ASSERT_EQ_JSON_STRINGS(j1, j2)                                         \
  ASSERT_EQ(nl::json::parse(j1), nl::json::parse(j2));

TEST(serialize_examples, ExceptionInfo) {
  const std::string expected = R"({
  "exceptionClass": "string",
  "errorCode": "string",
  "message": "string"
})";
  ExceptionInfo dto;
  dto.exceptionClass = "string";
  dto.errorCode = "string";
  dto.message = "string";

  std::string serialized = serialize(dto);
  ASSERT_EQ_JSON_STRINGS(serialized, expected);
}

// /{tenant}/controller/v1/{controllerId}/installedBase
TEST(serialize_examples, DdiAssignedVersion) {
  const std::string expected = R"({
  "name": "linux",
  "version": "1.2.3"
  })";
  DdiAssignedVersion dto;
  dto.name = "linux";
  dto.version = "1.2.3";

  std::string serialized = serialize(dto);
  ASSERT_EQ_JSON_STRINGS(serialized, expected);
}

// /{tenant}/controller/v1/{controllerId}/configData
TEST(serialize_examples, DdiConfigData) {
  const std::string expected = R"({
  "mode": "merge",
  "data": {
    "VIN": "JH4TB2H26CC000000",
    "hwRevision": "2"
  }
})";

  DdiConfigData dto;
  dto.mode = DdiConfigData::Mode::merge;
  dto.data["VIN"] = "JH4TB2H26CC000000";
  dto.data["hwRevision"] = "2";

  std::string serialized = serialize(dto);
  ASSERT_EQ_JSON_STRINGS(serialized, expected);
}

// /{tenant}/controller/v1/{controllerId}/deploymentBase/{actionId}/feedback
// /{tenant}/controller/v1/{controllerId}/cancelAction/{actionId}/feedback
TEST(serialize_examples, DdiActionFeedback) {
  const std::string expected = R"({
  "status": {
    "execution": "closed",
    "result": {
      "finished": "success",
      "progress": {
        "cnt": 2,
        "of": 5
      }
    },
    "code": 200,
    "details": [
      "Some feedback"
    ]
  },
  "timestamp": 1627997501890
})";

  DdiActionFeedback dto;
  dto.status.execution = DdiStatus::Execution::closed;
  dto.status.result.finished = DdiResult::success;
  dto.status.result.progress.emplace(2, 5);
  dto.status.code = 200;
  dto.status.details = {"Some feedback"};
  dto.timestamp = std::chrono::milliseconds(1627997501890);

  std::string serialized = serialize(dto);
  ASSERT_EQ_JSON_STRINGS(serialized, expected);
}

// /{tenant}/controller/v1/{controllerId}/confirmationBase/{actionId}/feedback
TEST(serialize_examples, DdiConfirmationFeedback) {
  const std::string expected = R"({
  "confirmation": "confirmed",
  "code": 200,
  "details": [
    "Feedback message"
  ]
})";

  DdiConfirmationFeedback dto;
  dto.confirmation = DdiConfirmationFeedback::Confirmation::confirmed;
  dto.code = 200;
  dto.details = {"Feedback message"};

  std::string serialized = serialize(dto);
  ASSERT_EQ_JSON_STRINGS(serialized, expected);
}

// /{tenant}/controller/v1/{controllerId}/confirmationBase/activateAutoConfirm
TEST(serialize_examples, DdiActivateAutoConfirmation) {
  const std::string expected = R"({
  "initiator": "exampleUser",
  "remark": "exampleRemark"
})";
  DdiActivateAutoConfirmation dto;
  dto.initiator = "exampleUser";
  dto.remark = "exampleRemark";

  std::string serialized = serialize(dto);
  ASSERT_EQ_JSON_STRINGS(serialized, expected);
}

// /{tenant}/controller/v1/{controllerId}
TEST(serialize_examples, DdiControllerBase) {
  const std::string expected = R"({ 
  "config": {
    "polling": {
      "sleep": "12:00:00"
    }
  },
  "_links": {
    "deploymentBase": {
      "href": "https://management-api.host.com/TENANT_ID/controller/v1/CONTROLLER_ID/deploymentBase/5?c=-2127183556"
    },
    "installedBase": {
      "href": "https://management-api.host.com/TENANT_ID/controller/v1/CONTROLLER_ID/installedBase/4"
    },
    "configData": {
      "href": "https://management-api.host.com/TENANT_ID/controller/v1/CONTROLLER_ID/configData"
    }
  }
})";
  DdiControllerBase dto;
  DdiConfig config;
  config.polling = {std::chrono::hours(12)};
  dto.config = config;
  Links links;
  links["deploymentBase"] =
      Link{"https://management-api.host.com/TENANT_ID/controller/v1/"
           "CONTROLLER_ID/deploymentBase/5?c=-2127183556"};
  links["installedBase"] = {"https://management-api.host.com/TENANT_ID/"
                            "controller/v1/CONTROLLER_ID/installedBase/4"};
  links["configData"] = {"https://management-api.host.com/TENANT_ID/controller/"
                         "v1/CONTROLLER_ID/configData"};
  dto.links = links;

  std::string serialized = serialize(dto);
  ASSERT_EQ_JSON_STRINGS(serialized, expected);
}

// /{tenant}/controller/v1/{controllerId}/softwaremodules/{softwareModuleId}/artifacts
TEST(serialize_examples, DdiArtifactList) {
  const std::string expected = R"([
  {
    "filename": "binaryFile",
    "hashes": {
      "sha1": "e4e667b70ff652cb9d9c8a49f141bd68e06cec6f",
      "md5": "13793b0e3a7830ed685d3ede7ff93048",
      "sha256": "c51368bf045803b429a67bdf04539a373d9fb8caa310fe0431265e6871b4f07a"
    },
    "size": 11,
    "_links": {
      "download": {
        "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/40/filename/binaryFile"
      },
      "download-http": {
        "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/40/filename/binaryFile"
      },
      "md5sum-http": {
        "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/40/filename/binaryFile.MD5SUM"
      },
      "md5sum": {
        "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/40/filename/binaryFile.MD5SUM"
      }
    }
  }
])";
  DdiArtifact artifact;
  artifact.filename = "binaryFile";
  DdiArtifactHash hashes;
  hashes.sha1 = "e4e667b70ff652cb9d9c8a49f141bd68e06cec6f";
  hashes.md5 = "13793b0e3a7830ed685d3ede7ff93048";
  hashes.sha256 =
      "c51368bf045803b429a67bdf04539a373d9fb8caa310fe0431265e6871b4f07a";
  artifact.hashes = hashes;
  artifact.size = 11;
  Links links;
  links["download"] = {
      "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/"
      "CONTROLLER_ID/softwaremodules/40/filename/binaryFile"};
  links["download-http"] = {
      "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/"
      "CONTROLLER_ID/softwaremodules/40/filename/binaryFile"};
  links["md5sum-http"] = {
      "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/"
      "CONTROLLER_ID/softwaremodules/40/filename/binaryFile.MD5SUM"};
  links["md5sum"] = {
      "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/"
      "CONTROLLER_ID/softwaremodules/40/filename/"
      "binaryFile.MD5SUM"};
  artifact.links = links;
  DdiArtifactList dto{artifact};

  std::string serialized = serialize(dto);
  ASSERT_EQ_JSON_STRINGS(serialized, expected);
}

// /{tenant}/controller/v1/{controllerId}/installedBase/{actionId}
// /{tenant}/controller/v1/{controllerId}/deploymentBase/{actionId}
TEST(serialize_examples, DdiDeploymentBase) {
  // Example payload was stripped to single chunk with single artifact
  const std::string expected = R"({
  "id": "8",
  "deployment": {
    "download": "forced",
    "update": "forced",
    "maintenanceWindow": "available",
    "chunks": [
      {
        "part": "bApp",
        "version": "1.0.91",
        "name": "oneapplication",
        "artifacts": [
          {
            "filename": "binary.tgz",
            "hashes": {
              "sha1": "e3ba7ff5839c210c98e254dde655147ffc49f5c9",
              "md5": "020017c498e6b0b8f76168fd55fa6fd1",
              "sha256": "80406288820379a82bbcbfbf7e8690146e46256f505de1c6d430c0168a74f6dd"
            },
            "size": 11,
            "_links": {
              "download": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/22/filename/binary.tgz"
              },
              "download-http": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/22/filename/binary.tgz"
              },
              "md5sum-http": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/22/filename/binary.tgz.MD5SUM"
              },
              "md5sum": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/22/filename/binary.tgz.MD5SUM"
              }
            }
          }
        ],
        "metadata": [
          {
            "key": "aMetadataKey",
            "value": "Metadata value as defined in software module"
          }
        ]
      }
    ]
  },
  "actionHistory": {
    "status": "RUNNING",
    "messages": [
      "Reboot",
      "Write firmware",
      "Download done",
      "Download failed. ErrorCode #5876745. Retry",
      "Started download",
      "Assignment initiated by user 'TestPrincipal'"
    ]
  }
})";

  DdiDeploymentBase dto;

  dto.id = "8";

  DdiDeployment deployment;
  deployment.download = DdiDeployment::Download::forced;
  deployment.update = DdiDeployment::Download::forced;
  deployment.maintenanceWindow = DdiDeployment::MaintenanceWindow::available;

  DdiChunk chunk;
  chunk.part = "bApp";
  chunk.version = "1.0.91";
  chunk.name = "oneapplication";
  chunk.metadata = DdiMetadata{
      {"aMetadataKey", "Metadata value as defined in software module"},
  };
  DdiArtifact artifact;
  artifact.filename = "binary.tgz";
  DdiArtifactHash hashes;
  hashes.sha1 = "e3ba7ff5839c210c98e254dde655147ffc49f5c9";
  hashes.md5 = "020017c498e6b0b8f76168fd55fa6fd1";
  hashes.sha256 =
      "80406288820379a82bbcbfbf7e8690146e46256f505de1c6d430c0168a74f6dd";
  artifact.hashes = hashes;
  artifact.size = 11;
  Links links;
  links["download"] = {
      "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/"
      "CONTROLLER_ID/softwaremodules/22/filename/binary.tgz"};
  links["download-http"] = {
      "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/"
      "CONTROLLER_ID/softwaremodules/22/filename/binary.tgz"};
  links["md5sum-http"] = {
      "https://link-to-cdn.com/api/v1/TENANT_ID/download/"
      "controller/CONTROLLER_ID/softwaremodules/22/filename/"
      "binary.tgz.MD5SUM"};
  links["md5sum"] = {"https://link-to-cdn.com/api/v1/TENANT_ID/download/"
                     "controller/CONTROLLER_ID/softwaremodules/22/"
                     "filename/binary.tgz.MD5SUM"};
  artifact.links = links;
  chunk.artifacts = {artifact};
  deployment.chunks.push_back(chunk);

  dto.deployment = deployment;

  DdiActionHistory history;
  history.status = "RUNNING";
  history.messages = {
      "Reboot",           "Write firmware",
      "Download done",    "Download failed. ErrorCode #5876745. Retry",
      "Started download", "Assignment initiated by user 'TestPrincipal'",
  };
  dto.actionHistory = history;

  std::string serialized = serialize(dto);
  ASSERT_EQ_JSON_STRINGS(serialized, expected);
}

// /{tenant}/controller/v1/{controllerId}/confirmationBase
TEST(serialize_examples, DdiConfirmationBase) {
  const std::string expected = R"({
  "autoConfirm": {
  "active": false
  },
  "_links": {
    "activateAutoConfirm": {
      "href": "https://management-api.host.com/TENANT_ID/controller/v1/CONTROLLER_ID/confirmationBase/activateAutoConfirm"
    },
    "confirmationBase": {
      "href": "https://management-api.host.com/TENANT_ID/controller/v1/CONTROLLER_ID/confirmationBase/10?c=-2122565939"
    }
  }
})";
  DdiConfirmationBase dto;

  dto.autoConfirm.active = false;
  Links links;
  links["activateAutoConfirm"] = {
      "https://management-api.host.com/TENANT_ID/controller/v1/CONTROLLER_ID/"
      "confirmationBase/activateAutoConfirm"};
  links["confirmationBase"] = {
      "https://management-api.host.com/TENANT_ID/controller/v1/"
      "CONTROLLER_ID/confirmationBase/10?c=-2122565939"};
  dto.links = links;

  std::string serialized = serialize(dto);
  ASSERT_EQ_JSON_STRINGS(serialized, expected);
}

// /{tenant}/controller/v1/{controllerId}/confirmationBase/{actionId}
TEST(serialize_examples, DdiConfirmationBaseAction) {
  // Example payload was stripped to single chunk with single artifact
  const std::string expected = R"({
  "id": "6",
  "confirmation": {
    "download": "forced",
    "update": "forced",
    "maintenanceWindow": "available",
    "chunks": [
      {
        "part": "bApp",
        "version": "1.0.96",
        "name": "oneapplication",
        "artifacts": [
          {
            "filename": "binary.tgz",
            "hashes": {
              "sha1": "701c0c0fcbee5e96fa5c5b819cb519686940ade3",
              "md5": "f0f6a34c4c9e79d07c2d92c3c3d88560",
              "sha256": "cff472a07c3143741fb03ac6c577acabef72a186a8bfaab00bbb47ca5ebbe554"
            },
            "size": 11,
            "_links": {
              "download": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/16/filename/binary.tgz"
              },
              "download-http": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/16/filename/binary.tgz"
              },
              "md5sum-http": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/16/filename/binary.tgz.MD5SUM"
              },
              "md5sum": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/16/filename/binary.tgz.MD5SUM"
              }
            }
          }
        ]
      }
    ]
  },
  "actionHistory": {
    "status": "WAIT_FOR_CONFIRMATION",
    "messages": [
      "Assignment initiated by user 'TestPrincipal'",
      "Waiting for the confirmation by the device before processing with the deployment"
    ]
  }
})";
  DdiConfirmationBaseAction dto;
  dto.id = "6";
  DdiDeployment confirmation;
  confirmation.download = DdiDeployment::Download::forced;
  confirmation.update = DdiDeployment::Update::forced;
  confirmation.maintenanceWindow = DdiDeployment::MaintenanceWindow::available;
  DdiChunk chunk;
  chunk.part = "bApp";
  chunk.version = "1.0.96";
  chunk.name = "oneapplication";
  DdiArtifact artifact;
  artifact.filename = "binary.tgz";
  DdiArtifactHash hashes;
  hashes.sha1 = "701c0c0fcbee5e96fa5c5b819cb519686940ade3";
  hashes.md5 = "f0f6a34c4c9e79d07c2d92c3c3d88560";
  hashes.sha256 =
      "cff472a07c3143741fb03ac6c577acabef72a186a8bfaab00bbb47ca5ebbe554";
  artifact.hashes = hashes;
  artifact.size = 11;
  Links links;
  links["download"] = {
      "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/"
      "CONTROLLER_ID/softwaremodules/16/filename/binary.tgz"};
  links["download-http"] = {
      "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/"
      "CONTROLLER_ID/softwaremodules/16/filename/binary.tgz"};
  links["md5sum-http"] = {
      "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/"
      "CONTROLLER_ID/softwaremodules/16/filename/binary.tgz.MD5SUM"};
  links["md5sum"] = {
      "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/"
      "CONTROLLER_ID/softwaremodules/16/filename/binary.tgz.MD5SUM"};
  artifact.links = links;
  chunk.artifacts = {artifact};
  confirmation.chunks = {chunk};
  dto.confirmation = confirmation;
  DdiActionHistory history;
  history.status = "WAIT_FOR_CONFIRMATION";
  history.messages = {
      "Assignment initiated by user 'TestPrincipal'",
      "Waiting for the confirmation by the device before processing with the "
      "deployment",
  };
  dto.actionHistory = history;

  std::string serialized = serialize(dto);
  ASSERT_EQ_JSON_STRINGS(serialized, expected);
}

// /{tenant}/controller/v1/{controllerId}/cancelAction/{actionId}
TEST(serialize_examples, DdiCancel) {
  const std::string expected = R"({
  "id": "11",
  "cancelAction": {
    "stopId": "11"
  }
})";
  DdiCancel dto;
  dto.id = "11";
  dto.cancelAction.stopId = "11";

  std::string serialized = serialize(dto);
  ASSERT_EQ_JSON_STRINGS(serialized, expected);
}
