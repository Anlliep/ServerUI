#pragma once
#include "serverRequests.h"

std::vector<std::string> connection::UpdateLogs(const std::string& type,
                                                const std::string& url) {
  URI uri(url);
  HTTPClientSession session(uri.getHost(), uri.getPort());

  std::string path;

  if (!type.empty()) {
    path = "/logs?type=" + type;
  } else {
    path = "/logs";
  }

  session.setKeepAliveTimeout(true);
  HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);

  int attempts = 0;
  while (attempts < 10) {
    try {
      session.sendRequest(request);
      break;
    } catch (const std::exception& ex) {
      std::cout << ex.what() << std::endl;
      ++attempts;
    }
  }

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

bool connection::IsServerActive(const std::string& url) {
  URI uri(url);
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

void connection::UpdateStatus(const std::string& name, const std::string& state,
                              const std::string& url) {
  URI uri(url);
  HTTPClientSession session(uri.getHost(), uri.getPort());

  session.setKeepAliveTimeout(true);
  HTTPRequest request(HTTPRequest::HTTP_PUT, name, HTTPMessage::HTTP_1_1);

  std::string status = "{\"status\" : \"" + state + "\"}";

  request.setContentLength(status.length());

  int attempts = 0;
  while (attempts < 10) {
    try {
      std::ostream& os = session.sendRequest(request);
      os << status;
      break;
    } catch (const std::exception& ex) {
      std::cout << ex.what() << std::endl;
      ++attempts;
    }
  }

  HTTPResponse response;
  if (response.getStatus() != 200) {
    throw std::exception("bad response");
  }
}

void connection::StartStopServer(const std::string& action) {
  URI uri("http://15.188.57.7:8008");
  HTTPClientSession session(uri.getHost(), uri.getPort());

  session.setKeepAliveTimeout(true);
  HTTPRequest request(HTTPRequest::HTTP_GET, action, HTTPMessage::HTTP_1_1);
  session.sendRequest(request);

  HTTPResponse response;
  if (response.getStatus() != 200) {
    throw std::exception("bad response");
  }
}

std::unordered_map<std::string, std::string> connection::UpdateMenuStatuses(
    const std::string& url) {
  URI uri(url);
  HTTPClientSession session(uri.getHost(), uri.getPort());

  std::string path = "/device";

  session.setKeepAliveTimeout(true);
  HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);

  int attempts = 0;
  while (attempts < 10) {
    try {
      session.sendRequest(request);
      break;
    } catch (const std::exception& ex) {
      std::cout << ex.what() << std::endl;
      ++attempts;
    }
  }

  HTTPResponse response;
  std::istream& page = session.receiveResponse(response);

  std::unordered_map<std::string, std::string> devices;
  Parser parser;
  Var name = parser.parse(page);
  Var status;
  Poco::JSON::Array::Ptr arr = name.extract<Poco::JSON::Array::Ptr>();

  Object::Ptr object;

  for (int i = 0; i < arr->size(); i++) {
    object = arr->getObject(i);
    name = object->get("name");
    status = object->get("status");
    devices.insert(std::make_pair(name.toString(), status.toString()));
  }
  return devices;
}
