#include <iostream>

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "ServerUI.h"

using namespace Poco;
using namespace Poco::Net;

int main() {


  

  myUI ui;
  //ui.Func();
  ui.Run();
  return 0;
}
