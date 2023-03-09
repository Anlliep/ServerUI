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
  while (attempts < 3) {
    try {
      session.sendRequest(request);
      break;
    } catch (const std::exception& ex) {
      std::cout << ex.what() << std::endl;
      ++attempts;
      if(attempts == 3) {
        std::vector<std::string> temp;
        temp.push_back("Error. 504 Gateway Timeout.");
        return temp;
      }
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

void connection::UpdateStatus(const std::string& id, const std::string& state,
                              const std::string& url) {
  URI uri(url);
  HTTPClientSession session(uri.getHost(), uri.getPort());

  session.setKeepAliveTimeout(true);
  HTTPRequest request(HTTPRequest::HTTP_PUT, id, HTTPMessage::HTTP_1_1);

  std::string status = "{\"status\" : \"" + state + "\"}";

  request.setContentLength(status.length());

  int attempts = 0;
  while (attempts < 3) {
    try {
      std::ostream& os = session.sendRequest(request);
      os << status;
      break;
    } catch (const std::exception& ex) {
      std::cout << ex.what() << std::endl;
      ++attempts;
    }
  }
}

void connection::StartStopServer(const std::string& action) {
  URI uri("http://15.188.57.7:8008");
  HTTPClientSession session(uri.getHost(), uri.getPort());

  session.setKeepAliveTimeout(true);
  HTTPRequest request(HTTPRequest::HTTP_GET, action, HTTPMessage::HTTP_1_1);

  try {
    session.sendRequest(request);
  } catch (const std::exception& ex) {
    std::cout << ex.what() << std::endl;
  }
}

std::vector<connection::Devices> connection::UpdateMenuStatuses(
    const std::string& url) {
  URI uri(url);
  HTTPClientSession session(uri.getHost(), uri.getPort());

  session.setKeepAliveTimeout(true);
  HTTPRequest request(HTTPRequest::HTTP_GET, "/device", HTTPMessage::HTTP_1_1);

  int attempts = 0;
  while (attempts < 3) {
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

  Parser parser;
  Var result = parser.parse(page);
  Poco::JSON::Array::Ptr arr = result.extract<Poco::JSON::Array::Ptr>();

  std::vector<connection::Devices> devices;

  for (int i = 0; i < arr->size(); i++) {
    Object::Ptr object = arr->getObject(i);
    connection::Devices temp;
    temp.floor = object->getValue<std::string>("floor");
    temp.id = object->getValue<std::string>("id");
    temp.name = object->getValue<std::string>("name");
    temp.room = object->getValue<std::string>("room");
    temp.status = object->getValue<std::string>("status");
    temp.type = object->getValue<std::string>("type");
    devices.push_back(temp);
  }
  return devices;
}
