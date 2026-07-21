// =====================================================
//
// Tests that parse examples from OpenAPI specification
//
// =====================================================

#include <gtest/gtest.h>

#include <hawkbit/dto/common.hpp>
#include <hawkbit/dto/definitions.hpp>

using namespace hawkbit::dto;

TEST(parse_examples, ExceptionInfo) {
  const std::string raw = R"({
  "exceptionClass": "string",
  "errorCode": "string",
  "message": "string",
  "info": {
    "additionalProp1": {},
    "additionalProp2": {},
    "additionalProp3": {}
  }
})";

  auto dto = parse<ExceptionInfo>(raw);

  EXPECT_EQ(dto.exceptionClass, "string");
  EXPECT_EQ(dto.errorCode, "string");
  EXPECT_EQ(dto.message, "string");
}

// /{tenant}/controller/v1/{controllerId}/installedBase
TEST(parse_examples, DdiAssignedVersion) {
  const std::string raw = R"({
  "name": "linux",
  "version": "1.2.3"
  })";

  auto dto = parse<DdiAssignedVersion>(raw);

  EXPECT_EQ(dto.name, "linux");
  EXPECT_EQ(dto.version, "1.2.3");
}

// /{tenant}/controller/v1/{controllerId}/configData
TEST(parse_examples, DdiConfigData) {
  const std::string raw = R"({
  "mode": "merge",
  "data": {
    "VIN": "JH4TB2H26CC000000",
    "hwRevision": "2"
  }
})";

  auto dto = parse<DdiConfigData>(raw);

  ASSERT_TRUE(dto.mode.has_value());
  EXPECT_EQ(dto.mode.value(), DdiConfigData::Mode::merge);
  ASSERT_EQ(dto.data.size(), 2);
  ASSERT_TRUE(dto.data.find("VIN") != dto.data.end());
  EXPECT_EQ(dto.data["VIN"], "JH4TB2H26CC000000");
  ASSERT_TRUE(dto.data.find("hwRevision") != dto.data.end());
  EXPECT_EQ(dto.data["hwRevision"], "2");
}

// /{tenant}/controller/v1/{controllerId}/deploymentBase/{actionId}/feedback
// /{tenant}/controller/v1/{controllerId}/cancelAction/{actionId}/feedback
TEST(parse_examples, DdiActionFeedback) {
  const std::string raw = R"({
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

  auto dto = parse<DdiActionFeedback>(raw);

  EXPECT_EQ(dto.status.execution, DdiStatus::Execution::closed);
  EXPECT_EQ(dto.status.result.finished, DdiResult::Finished::success);
  ASSERT_TRUE(dto.status.result.progress.has_value());
  EXPECT_EQ(dto.status.result.progress.value().cnt, 2);
  ASSERT_TRUE(dto.status.result.progress.value().of.has_value());
  EXPECT_EQ(dto.status.result.progress.value().of.value(), 5);
  ASSERT_TRUE(dto.status.code.has_value());
  EXPECT_EQ(dto.status.code.value(), 200);
  ASSERT_TRUE(dto.status.details.has_value());
  ASSERT_EQ(dto.status.details.value().size(), 1);
  EXPECT_EQ(dto.status.details.value()[0], "Some feedback");
  ASSERT_TRUE(dto.timestamp.has_value());
  EXPECT_EQ(dto.timestamp.value(), std::chrono::milliseconds(1627997501890));
}

// /{tenant}/controller/v1/{controllerId}/confirmationBase/{actionId}/feedback
TEST(parse_examples, DdiConfirmationFeedback) {
  const std::string raw = R"({
  "confirmation": "confirmed",
  "code": 200,
  "details": [
    "Feedback message"
  ]
})";

  auto dto = parse<DdiConfirmationFeedback>(raw);

  EXPECT_EQ(dto.confirmation, DdiConfirmationFeedback::Confirmation::confirmed);
  ASSERT_TRUE(dto.code.has_value());
  EXPECT_EQ(dto.code.value(), 200);
  ASSERT_TRUE(dto.details.has_value());
  ASSERT_EQ(dto.details.value().size(), 1);
  EXPECT_EQ(dto.details.value()[0], "Feedback message");
}

// /{tenant}/controller/v1/{controllerId}/confirmationBase/activateAutoConfirm
TEST(parse_examples, DdiActivateAutoConfirmation) {
  const std::string raw = R"({
  "initiator": "exampleUser",
  "remark": "exampleRemark"
})";

  auto dto = parse<DdiActivateAutoConfirmation>(raw);

  ASSERT_TRUE(dto.initiator.has_value());
  EXPECT_EQ(dto.initiator.value(), "exampleUser");
  ASSERT_TRUE(dto.remark.has_value());
  EXPECT_EQ(dto.remark.value(), "exampleRemark");
}

// /{tenant}/controller/v1/{controllerId}
TEST(parse_examples, DdiControllerBase) {
  const std::string raw = R"({ 
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
  auto dto = parse<DdiControllerBase>(raw);

  ASSERT_TRUE(dto.config.has_value());
  auto &config = dto.config.value();
  ASSERT_TRUE(config.polling.has_value());
  auto &polling = config.polling.value();
  EXPECT_EQ(polling.sleep, std::chrono::hours(12));

  ASSERT_TRUE(dto.links.has_value());
  auto &links = dto.links.value();
  ASSERT_TRUE(links.find("deploymentBase") != links.end());
  EXPECT_EQ(links["deploymentBase"].href.value_or("NO_VALUE"),
            "https://management-api.host.com/TENANT_ID/controller/v1/"
            "CONTROLLER_ID/deploymentBase/5?c=-2127183556");
  ASSERT_TRUE(links.find("installedBase") != links.end());
  EXPECT_EQ(links["installedBase"].href.value_or("NO_VALUE"),
            "https://management-api.host.com/TENANT_ID/controller/v1/"
            "CONTROLLER_ID/installedBase/4");
  ASSERT_TRUE(links.find("configData") != links.end());
  EXPECT_EQ(links["configData"].href.value_or("NO_VALUE"),
            "https://management-api.host.com/TENANT_ID/"
            "controller/v1/CONTROLLER_ID/configData");
}

// /{tenant}/controller/v1/{controllerId}/softwaremodules/{softwareModuleId}/artifacts
TEST(parse_examples, DdiArtifactList) {
  const std::string raw = R"([
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

  auto dto = parse<DdiArtifactList>(raw);

  ASSERT_EQ(dto.size(), 1);

  auto &artifact = dto.at(0);
  EXPECT_EQ(artifact.filename, "binaryFile");

  ASSERT_TRUE(artifact.hashes.has_value());
  auto &hashes = artifact.hashes.value();
  ASSERT_TRUE(hashes.sha1.has_value());
  EXPECT_EQ(hashes.sha1.value(), "e4e667b70ff652cb9d9c8a49f141bd68e06cec6f");
  ASSERT_TRUE(hashes.md5.has_value());
  EXPECT_EQ(hashes.md5.value(), "13793b0e3a7830ed685d3ede7ff93048");
  ASSERT_TRUE(hashes.sha256.has_value());
  EXPECT_EQ(hashes.sha256.value(),
            "c51368bf045803b429a67bdf04539a373d9fb8caa310fe0431265e6871b4f07a");

  ASSERT_TRUE(artifact.size.has_value());
  EXPECT_EQ(artifact.size.value(), 11);

  ASSERT_TRUE(artifact.links.has_value());
  auto &links = artifact.links.value();
  ASSERT_TRUE(links.find("download") != links.end());
  EXPECT_EQ(links["download"].href.value_or("NO_VALUE"),
            "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/"
            "CONTROLLER_ID/softwaremodules/40/filename/binaryFile");
  ASSERT_TRUE(links.find("download-http") != links.end());
  EXPECT_EQ(links["download-http"].href.value_or("NO_VALUE"),
            "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/"
            "CONTROLLER_ID/softwaremodules/40/filename/binaryFile");
  ASSERT_TRUE(links.find("md5sum-http") != links.end());
  EXPECT_EQ(links["md5sum-http"].href.value_or("NO_VALUE"),
            "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/"
            "CONTROLLER_ID/softwaremodules/40/filename/binaryFile.MD5SUM");
  ASSERT_TRUE(links.find("md5sum") != links.end());
  EXPECT_EQ(links["md5sum"].href.value_or("NO_VALUE"),
            "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/"
            "CONTROLLER_ID/softwaremodules/40/filename/binaryFile.MD5SUM");
}

// /{tenant}/controller/v1/{controllerId}/installedBase/{actionId}
// /{tenant}/controller/v1/{controllerId}/deploymentBase/{actionId}
TEST(parse_examples, DdiDeploymentBase) {
  const std::string raw = R"({
  "id": "8",
  "deployment": {
    "download": "forced",
    "update": "forced",
    "maintenanceWindow": "available",
    "chunks": [
      {
        "part": "jvm",
        "version": "1.0.75",
        "name": "oneapp runtime",
        "artifacts": [
          {
            "filename": "binary.tgz",
            "hashes": {
              "sha1": "986a1ade8b8a2f758ce951340cc5e21335cc2a00",
              "md5": "d04440e6533863247655ac5fd4345bcc",
              "sha256": "b3a04740a19e36057ccf258701922f3cd2f1a880536be53a3ca8d50f6b615975"
            },
            "size": 13,
            "_links": {
              "download": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/23/filename/binary.tgz"
              },
              "download-http": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/23/filename/binary.tgz"
              },
              "md5sum-http": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/23/filename/binary.tgz.MD5SUM"
              },
              "md5sum": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/23/filename/binary.tgz.MD5SUM"
              }
            }
          },
          {
            "filename": "file.signature",
            "hashes": {
              "sha1": "986a1ade8b8a2f758ce951340cc5e21335cc2a00",
              "md5": "d04440e6533863247655ac5fd4345bcc",
              "sha256": "b3a04740a19e36057ccf258701922f3cd2f1a880536be53a3ca8d50f6b615975"
            },
            "size": 13,
            "_links": {
              "download": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/23/filename/file.signature"
              },
              "download-http": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/23/filename/file.signature"
              },
              "md5sum-http": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/23/filename/file.signature.MD5SUM"
              },
              "md5sum": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/23/filename/file.signature.MD5SUM"
              }
            }
          }
        ]
      },
      {
        "part": "os",
        "version": "1.0.79",
        "name": "one Firmware",
        "artifacts": [
          {
            "filename": "binary.tgz",
            "hashes": {
              "sha1": "574cd34be20f75d101ed23518339cc38c5157bdb",
              "md5": "a0637c1ccb9fd53e2ba6f45712516989",
              "sha256": "498014801aab66be1d7fbea56b1aa5959651b6fd710308e196a8c414029e7291"
            },
            "size": 13,
            "_links": {
              "download": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/24/filename/binary.tgz"
              },
              "download-http": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/24/filename/binary.tgz"
              },
              "md5sum-http": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/24/filename/binary.tgz.MD5SUM"
              },
              "md5sum": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/24/filename/binary.tgz.MD5SUM"
              }
            }
          },
          {
            "filename": "file.signature",
            "hashes": {
              "sha1": "574cd34be20f75d101ed23518339cc38c5157bdb",
              "md5": "a0637c1ccb9fd53e2ba6f45712516989",
              "sha256": "498014801aab66be1d7fbea56b1aa5959651b6fd710308e196a8c414029e7291"
            },
            "size": 13,
            "_links": {
              "download": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/24/filename/file.signature"
              },
              "download-http": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/24/filename/file.signature"
              },
              "md5sum-http": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/24/filename/file.signature.MD5SUM"
              },
              "md5sum": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/24/filename/file.signature.MD5SUM"
              }
            }
          }
        ]
      },
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
          },
          {
            "filename": "file.signature",
            "hashes": {
              "sha1": "e3ba7ff5839c210c98e254dde655147ffc49f5c9",
              "md5": "020017c498e6b0b8f76168fd55fa6fd1",
              "sha256": "80406288820379a82bbcbfbf7e8690146e46256f505de1c6d430c0168a74f6dd"
            },
            "size": 11,
            "_links": {
              "download": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/22/filename/file.signature"
              },
              "download-http": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/22/filename/file.signature"
              },
              "md5sum-http": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/22/filename/file.signature.MD5SUM"
              },
              "md5sum": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/22/filename/file.signature.MD5SUM"
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

  auto dto = parse<DdiDeploymentBase>(raw);

  EXPECT_EQ(dto.id, "8");

  const auto &deployment = dto.deployment;
  ASSERT_TRUE(deployment.download.has_value());
  EXPECT_EQ(deployment.download.value(), DdiDeployment::Download::forced);
  ASSERT_TRUE(deployment.update.has_value());
  EXPECT_EQ(deployment.update.value(), DdiDeployment::Update::forced);
  EXPECT_EQ(deployment.maintenanceWindow.value(),
            DdiDeployment::MaintenanceWindow::available);
  ASSERT_TRUE(deployment.maintenanceWindow.has_value());

  EXPECT_EQ(deployment.chunks.size(), 3);
  const auto &chunk = deployment.chunks.at(2);
  EXPECT_EQ(chunk.part, "bApp");
  EXPECT_EQ(chunk.version, "1.0.91");
  EXPECT_EQ(chunk.name, "oneapplication");
  ASSERT_TRUE(chunk.metadata.has_value());
  ASSERT_EQ(chunk.metadata.value().size(), 1);
  EXPECT_EQ(chunk.metadata.value().at(0).key, "aMetadataKey");
  EXPECT_EQ(chunk.metadata.value().at(0).value,
            "Metadata value as defined in software module");

  ASSERT_TRUE(chunk.artifacts.has_value());
  ASSERT_EQ(chunk.artifacts.value().size(), 2);

  const auto &artifact = chunk.artifacts.value().at(0);
  EXPECT_EQ(artifact.filename, "binary.tgz");
  // Parsing artifacts is tested in other tests

  ASSERT_TRUE(dto.actionHistory.has_value());
  const auto &history = dto.actionHistory.value();
  ASSERT_TRUE(history.status.has_value());
  ASSERT_EQ(history.status, "RUNNING");
  ASSERT_TRUE(history.messages.has_value());
  ASSERT_EQ(history.messages.value().size(), 6);
}

// /{tenant}/controller/v1/{controllerId}/confirmationBase
TEST(parse_examples, DdiConfirmationBase) {
  const std::string raw = R"({
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

  auto dto = parse<DdiConfirmationBase>(raw);

  ASSERT_TRUE(dto.autoConfirm.active == false);
  ASSERT_TRUE(dto.links.has_value());
  auto &links = dto.links.value();
  ASSERT_TRUE(links.find("activateAutoConfirm") != links.end());
  EXPECT_EQ(links["activateAutoConfirm"].href.value_or("NO_VALUE"),
            "https://management-api.host.com/TENANT_ID/controller/v1/"
            "CONTROLLER_ID/confirmationBase/activateAutoConfirm");
  ASSERT_TRUE(links.find("confirmationBase") != links.end());
  EXPECT_EQ(links["confirmationBase"].href.value_or("NO_VALUE"),
            "https://management-api.host.com/TENANT_ID/controller/v1/"
            "CONTROLLER_ID/confirmationBase/10?c=-2122565939");
}

// /{tenant}/controller/v1/{controllerId}/confirmationBase/{actionId}
TEST(parse_examples, DdiConfirmationBaseAction) {
  const std::string raw = R"({
  "id": "6",
  "confirmation": {
    "download": "forced",
    "update": "forced",
    "maintenanceWindow": "available",
    "chunks": [
      {
        "part": "jvm",
        "version": "1.0.62",
        "name": "oneapp runtime",
        "artifacts": [
          {
            "filename": "binary.tgz",
            "hashes": {
              "sha1": "3dceccec02e7626184bdbba12b247b67ff04c363",
              "md5": "a9a7df0aa4c72b3b03b654c42d29744b",
              "sha256": "971d8db88fef8e7a3e6d5bbf501d69b07d0c300d9be948aff8b52960ef039358"
            },
            "size": 11,
            "_links": {
              "download": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/17/filename/binary.tgz"
              },
              "download-http": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/17/filename/binary.tgz"
              },
              "md5sum-http": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/17/filename/binary.tgz.MD5SUM"
              },
              "md5sum": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/17/filename/binary.tgz.MD5SUM"
              }
            }
          },
          {
            "filename": "file.signature",
            "hashes": {
              "sha1": "3dceccec02e7626184bdbba12b247b67ff04c363",
              "md5": "a9a7df0aa4c72b3b03b654c42d29744b",
              "sha256": "971d8db88fef8e7a3e6d5bbf501d69b07d0c300d9be948aff8b52960ef039358"
            },
            "size": 11,
            "_links": {
              "download": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/17/filename/file.signature"
              },
              "download-http": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/17/filename/file.signature"
              },
              "md5sum-http": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/17/filename/file.signature.MD5SUM"
              },
              "md5sum": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/17/filename/file.signature.MD5SUM"
              }
            }
          }
        ]
      },
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
          },
          {
            "filename": "file.signature",
            "hashes": {
              "sha1": "701c0c0fcbee5e96fa5c5b819cb519686940ade3",
              "md5": "f0f6a34c4c9e79d07c2d92c3c3d88560",
              "sha256": "cff472a07c3143741fb03ac6c577acabef72a186a8bfaab00bbb47ca5ebbe554"
            },
            "size": 11,
            "_links": {
              "download": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/16/filename/file.signature"
              },
              "download-http": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/16/filename/file.signature"
              },
              "md5sum-http": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/16/filename/file.signature.MD5SUM"
              },
              "md5sum": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/16/filename/file.signature.MD5SUM"
              }
            }
          }
        ]
      },
      {
        "part": "os",
        "version": "1.0.44",
        "name": "one Firmware",
        "artifacts": [
          {
            "filename": "binary.tgz",
            "hashes": {
              "sha1": "2b09765e953cd138b7da8f4725e48183dab62aec",
              "md5": "9b0aa2f51379cb4a5e0b7d026c2605c9",
              "sha256": "618faa741070b3f8148bad06f088e537a8f7913e734df4dde61fb163725cb4ee"
            },
            "size": 15,
            "_links": {
              "download": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/18/filename/binary.tgz"
              },
              "download-http": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/18/filename/binary.tgz"
              },
              "md5sum-http": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/18/filename/binary.tgz.MD5SUM"
              },
              "md5sum": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/18/filename/binary.tgz.MD5SUM"
              }
            }
          },
          {
            "filename": "file.signature",
            "hashes": {
              "sha1": "2b09765e953cd138b7da8f4725e48183dab62aec",
              "md5": "9b0aa2f51379cb4a5e0b7d026c2605c9",
              "sha256": "618faa741070b3f8148bad06f088e537a8f7913e734df4dde61fb163725cb4ee"
            },
            "size": 15,
            "_links": {
              "download": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/18/filename/file.signature"
              },
              "download-http": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/18/filename/file.signature"
              },
              "md5sum-http": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/18/filename/file.signature.MD5SUM"
              },
              "md5sum": {
                "href": "https://link-to-cdn.com/api/v1/TENANT_ID/download/controller/CONTROLLER_ID/softwaremodules/18/filename/file.signature.MD5SUM"
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
    "status": "WAIT_FOR_CONFIRMATION",
    "messages": [
      "Assignment initiated by user 'TestPrincipal'",
      "Waiting for the confirmation by the device before processing with the deployment"
    ]
  }
})";

  auto dto = parse<DdiConfirmationBaseAction>(raw);

  EXPECT_EQ(dto.id, "6");

  ASSERT_FALSE(dto.links.has_value());

  ASSERT_EQ(dto.confirmation.download, DdiDeployment::Download::forced);
  ASSERT_EQ(dto.confirmation.update, DdiDeployment::Download::forced);
  ASSERT_EQ(dto.confirmation.maintenanceWindow,
            DdiDeployment::MaintenanceWindow::available);
  ASSERT_EQ(dto.confirmation.chunks.size(), 3);
  // Parsing of chunks is done in other test, here it is skipped

  ASSERT_TRUE(dto.actionHistory.has_value());
  auto actionHistory = dto.actionHistory.value();
  ASSERT_TRUE(actionHistory.status.has_value());
  EXPECT_EQ(actionHistory.status.value(), "WAIT_FOR_CONFIRMATION");
  ASSERT_TRUE(actionHistory.messages.has_value());
  EXPECT_EQ(actionHistory.messages.value().size(), 2);
}

// /{tenant}/controller/v1/{controllerId}/cancelAction/{actionId}
TEST(parse_examples, DdiCancel) {
  const std::string raw = R"({
  "id": "11",
  "cancelAction": {
    "stopId": "11"
  }
})";

  auto dto = parse<DdiCancel>(raw);

  ASSERT_TRUE(dto.id.has_value());
  EXPECT_EQ(dto.id.value(), "11");
  EXPECT_EQ(dto.cancelAction.stopId, "11");
}
