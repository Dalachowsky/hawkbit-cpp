#include "HttpClient.hpp"

#include <string>

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

using namespace Poco::Net;

namespace hawkbit::api {

HttpClient::HttpClient(const std::string &host, unsigned int port)
    : client(host, port) {}

void HttpClient::setCredentials(const std::string &scheme,
                                const std::string &authInfo) {
  credentials = Credentials{scheme, authInfo};
}

HttpResponse HttpClient::getString(const std::string &uri) {
  HTTPRequest req(HTTPRequest::HTTP_GET, uri);
  if (credentials.has_value()) {
    req.setCredentials(credentials.value().scheme,
                       credentials.value().authInfo);
  }
  client.sendRequest(req);

  HTTPResponse res;
  std::istream &rs = client.receiveResponse(res);
  std::ostringstream ss;
  ss << rs.rdbuf();

  HttpResponse ret;
  ret.code = res.getStatus();
  ret.data = ss.str();
  return ret;
}

HttpResponse HttpClient::putJson(const std::string &uri,
                                 const std::string &jsonString) {
  HTTPRequest req(HTTPRequest::HTTP_PUT, uri);
  if (credentials.has_value()) {
    req.setCredentials(credentials.value().scheme,
                       credentials.value().authInfo);
  }
  req.setContentType("application/json");
  req.setContentLength(jsonString.size());
  std::ostream &os = client.sendRequest(req);
  os.write(jsonString.data(), jsonString.size());

  HTTPResponse res;
  std::istream &rs = client.receiveResponse(res);
  std::ostringstream ss;
  ss << rs.rdbuf();

  HttpResponse ret;
  ret.code = res.getStatus();
  ret.data = ss.str();
  return ret;
}

} // namespace hawkbit::api
