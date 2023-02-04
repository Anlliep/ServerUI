#pragma once
#include "serverRequests.h"

std::vector<std::string> connection::GetLogs() {
  URI uri("http://15.188.57.7:8080");
  HTTPClientSession session(uri.getHost(), uri.getPort());

  std::string path = "/logs";

  session.setKeepAliveTimeout(true);
  HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
  session.sendRequest(request);

  HTTPResponse response;
  if (response.getStatus() != 200) {
    throw std::exception("bad response");
  }
  std::istream& page = session.receiveResponse(response);

  std::vector<std::string> logs;
  std::string line{};
  std::string str{};

  while (std::getline(page, line)) {
    std::stringstream strStream(line);
    while (std::getline(strStream, str, ',')) {
      logs.push_back(str);
    }
  }
  return logs;
}

void connection::UpdateWindowsStatus(const std::string& state) {
   URI uri("http://15.188.57.7:8080");
  HTTPClientSession session(uri.getHost(), uri.getPort());

  std::string path = "/device?id=2";

  session.setKeepAliveTimeout(true);
  HTTPRequest request(HTTPRequest::HTTP_PUT, path, HTTPMessage::HTTP_1_1);

  std::string status = "{\"status\" : \"" + state + "\"}";

  request.setContentLength(status.length());
  std::ostream& os = session.sendRequest(request);
  os << status;

  HTTPResponse response;
  if (response.getStatus() != 200) {
    throw std::exception("bad response");
  }
}
