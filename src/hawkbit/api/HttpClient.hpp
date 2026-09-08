#pragma once

#include <string>
#include <variant>

#include <Poco/Net/HTTPClientSession.h>
#include <nlohmann/json.hpp>

namespace nl = nlohmann;

namespace hawkbit::api {

struct HttpResponse {
  int code;
  std::variant<std::string> data;

  bool isSuccess() const { return code >= 200 && code < 300; }
};

class HttpClient {
public:
  HttpClient(const std::string &host, unsigned int port);

  void setCredentials(const std::string &scheme, const std::string &authInfo);

  HttpResponse putJson(const std::string &uri, const std::string &jsonString);
  HttpResponse getString(const std::string &uri);

private:
  struct Credentials {
    std::string scheme;
    std::string authInfo;
  };

  Poco::Net::HTTPClientSession client;
  std::optional<Credentials> credentials;
};

}; // namespace hawkbit::api
