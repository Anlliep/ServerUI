#pragma once

#include <iostream>
#include <unordered_map>
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
struct Devices {
  std::string floor;
  std::string id;
  std::string name;
  std::string room;
  std::string status;
  std::string type;
};

std::vector<std::string> UpdateLogs(const std::string&, const std::string&);
bool IsServerActive(const std::string&);
void UpdateStatus(const std::string&, const std::string&, const std::string&);
void StartStopServer(const std::string&);
std::vector<Devices> UpdateMenuStatuses(const std::string&);
}  // namespace connection
