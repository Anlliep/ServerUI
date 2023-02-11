#pragma once
#include <iostream>
#include <vector>

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"

using namespace Poco;
using namespace Poco::Net;

namespace connection {
std::vector<std::string> UpdateLogs(const std::string&);
bool IsServerActive();
void UpdateStatus(const std::string&, const std::string&);
void StartStopServer(const std::string&, const std::string&);
}  // namespace connection
