﻿#pragma once
#include "serverRequests.h"

std::vector<std::string> connection::UpdateLogs(const std::string& type) {
  URI uri("http://15.188.57.7:8080");
  HTTPClientSession session(uri.getHost(), uri.getPort());

  std::string path;

  if (!type.empty()) {
    path = "/logs?type=" + type;
  } else {
    path = "/logs";
  }

  session.setKeepAliveTimeout(true);
  HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
  session.sendRequest(request);

  HTTPResponse response;
  std::istream& page = session.receiveResponse(response);

  std::vector<std::string> logs{};
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

bool connection::IsServerActive() {
  URI uri("http://15.188.57.7:8080");
  HTTPClientSession session(uri.getHost(), uri.getPort());

  session.setKeepAliveTimeout(true);
  HTTPRequest request(HTTPRequest::HTTP_GET, "/", HTTPMessage::HTTP_1_1);

  try {
    session.sendRequest(request);
    return true;
  } catch (...) {
    return false;
  }
}

void connection::UpdateStatus(const std::string& id, const std::string& state) {
  URI uri("http://15.188.57.7:8080");
  HTTPClientSession session(uri.getHost(), uri.getPort());

  session.setKeepAliveTimeout(true);
  HTTPRequest request(HTTPRequest::HTTP_PUT, id, HTTPMessage::HTTP_1_1);

  std::string status = "{\"status\" : \"" + state + "\"}";

  request.setContentLength(status.length());

  std::ostream& os = session.sendRequest(request);
  os << status;

  HTTPResponse response;
  if (response.getStatus() != 200) {
    throw std::exception("bad response");
  }
}

void connection::StartStopServer(const std::string& ip,
                                 const std::string& action) {
  URI uri(ip);
  HTTPClientSession session(uri.getHost(), uri.getPort());

  std::string path = action;

  session.setKeepAliveTimeout(true);
  HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
  session.sendRequest(request);

  HTTPResponse response;
  if (response.getStatus() != 200) {
    throw std::exception("bad response");
  }
}