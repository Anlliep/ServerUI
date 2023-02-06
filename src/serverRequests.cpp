#pragma once
#include "serverRequests.h"

void connection::StartServer() { 
  URI uri("http://15.188.57.7:8008");
  HTTPClientSession session(uri.getHost(), uri.getPort());

  std::string path = "/start";

  session.setKeepAliveTimeout(true);
  HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
  session.sendRequest(request);
}

void connection::StopServer() {
  URI uri("http://15.188.57.7:8008");
  HTTPClientSession session(uri.getHost(), uri.getPort());

  std::string path = "/stop";

  session.setKeepAliveTimeout(true);
  HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
  session.sendRequest(request);
}

std::vector<std::string> connection::GetLogs() {
  URI uri("http://15.188.57.7:8080");
  HTTPClientSession session(uri.getHost(), uri.getPort());

  std::string path = "/logs";

  session.setKeepAliveTimeout(true);
  HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
  session.sendRequest(request);

  HTTPResponse response;
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
