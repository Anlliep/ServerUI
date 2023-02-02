#include <iostream>

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/URI.h"
#include "ServerUI.h"

using namespace Poco;
using namespace Poco::Net;

int main() {
  std::cout << "f";
  URI uri("http://15.188.57.7:8080");
  HTTPClientSession session(uri.getHost(), uri.getPort());
  std::string path = "/logs";

  session.setKeepAliveTimeout(true);

  HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);

  session.sendRequest(request);

  HTTPResponse response;

  std::istream& page = session.receiveResponse(response);

  std::cout << response.getStatus() << std::endl;

  //myUI ui;
  //ui.Run();
  return 0;
}
