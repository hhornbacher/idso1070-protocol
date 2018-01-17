#ifndef _SERVER_H_
#define _SERVER_H_

#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <unistd.h>

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>

#include "json.h"
using json = nlohmann::json;

#include "Protocol.h"

class Server
{
public:
  static const int Port = 8000;
  static const int ResponseBufferSize = 1024 * 16;

private:
  MHD_Daemon *daemon;

  const char *usbDevice = "/dev/ttyACM0";
  USBConnector usbConnection;

  CommandFactory cmdFactory;
  Protocol protocol;

  float progress = 0;

  int handleRequest(MHD_Connection *connection, const char *url, const char *method);

  int sendResponse(MHD_Connection *connection, json &j);

  int statusResponse(MHD_Connection *connection);
  int deviceResponse(MHD_Connection *connection);
  int channel1Response(MHD_Connection *connection);
  int channel2Response(MHD_Connection *connection);
  int triggerResponse(MHD_Connection *connection);

  void onProgress(float progress);

  static int _handleRequest(void *cls, MHD_Connection *connection,
                            const char *url,
                            const char *method, const char *version,
                            const char *upload_data,
                            size_t *upload_data_size, void **con_cls);

public:
  Server();

  void start();
  void stop();
  void process();
};

#endif // _SERVER_H_