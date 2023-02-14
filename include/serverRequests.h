#pragma once

#include <iostream>
#include <vector>

#include "Poco/Dynamic/Var.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/JSON.h "
#include "Poco/JSON/Parser.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"

using namespace Poco;
using namespace Poco::Net;
using namespace Poco::JSON;
using namespace Poco::Dynamic;

namespace connection {
struct MenuStatuses {
  bool isWindows = false;
  bool isLight = false;
  bool isDevice = false;
  bool isOven = false;
  bool isTap = false;
  bool isGarage = false;
};

std::vector<std::string> UpdateLogs(const std::string&, const std::string&);
bool IsServerActive(const std::string&);
void UpdateStatus(const std::string&, const std::string&, const std::string&);
void StartStopServer(const std::string&);
MenuStatuses UpdateMenuStatuses(const std::string&);
}  // namespace connection
