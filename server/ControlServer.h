#ifndef _CONTROL_SERVER_H_
#define _CONTROL_SERVER_H_

#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <unistd.h>

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

  void statusRequestHandler(HTTPServerRequest &req, HTTPServerResponse &resp);

  void deviceRequestHandler(HTTPServerRequest &req, HTTPServerResponse &resp);
  void channel1RequestHandler(HTTPServerRequest &req, HTTPServerResponse &resp);
  void channel2RequestHandler(HTTPServerRequest &req, HTTPServerResponse &resp);
  void triggerRequestHandler(HTTPServerRequest &req, HTTPServerResponse &resp);

  void controlRequestHandler(HTTPServerRequest &req, HTTPServerResponse &resp);

  void usbListRequestHandler(HTTPServerRequest &req, HTTPServerResponse &resp);
  void usbConnectRequestHandler(HTTPServerRequest &req, HTTPServerResponse &resp);

  void wifiConnectRequestHandler(HTTPServerRequest &req, HTTPServerResponse &resp);

  void onProgress(float progress);

public:
  ControlServer();

  void start();
  void stop();
  void process();
};

#endif // _CONTROL_SERVER_H_