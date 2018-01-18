#ifndef _CONTROL_SERVER_H_
#define _CONTROL_SERVER_H_

#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <unistd.h>

#include "json.h"
using json = nlohmann::json;

#include "Protocol.h"
#include "HttpServer.h"
#include "StreamServer.h"

class ControlServer
{
public:
  static const int HttpPort = 8800;
  static const int StreamChannel1Port = 8810;
  static const int StreamChannel2Port = 8820;

private:
  const char *usbDevice = "/dev/ttyACM0";
  USBConnector usbConnection;

  CommandFactory cmdFactory;
  Protocol protocol;

  HttpServer httpServer;
  StreamServer channel1StreamServer;
  StreamServer channel2StreamServer;

  float progress = 0;

  int statusRequestHandler(HttpServer::Connection connection, json *);
  int deviceRequestHandler(HttpServer::Connection connection, json *);
  int channel1RequestHandler(HttpServer::Connection connection, json *);
  int channel2RequestHandler(HttpServer::Connection connection, json *);
  int triggerRequestHandler(HttpServer::Connection connection, json *);
  int controlRequestHandler(HttpServer::Connection connection, json *);

  void onProgress(float progress);

public:
  ControlServer();

  void start();
  void stop();
  void process();
};

#endif // _CONTROL_SERVER_H_