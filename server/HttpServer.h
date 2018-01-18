#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_

#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <unistd.h>

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>

#include <functional>

#include <microhttpd.h>

#include "json.h"

using json = nlohmann::json;
using namespace std;

enum HttpMethod
{
  METHOD_GET,
  METHOD_POST,
  METHOD_PUT,
  METHOD_DELETE
};

class HttpServer
{
public:
  static const int ResponseBufferSize = 1024 * 16;
  static const int PostBufferSize = 1024 * 16;

  typedef MHD_Connection *Connection;
  typedef function<int(Connection, json *)> RequestHandler;
  typedef map<string, RequestHandler> ServerRoutes;

private:
  struct ConnectionInfo
  {
    HttpMethod method;
    char *answerString;
    MHD_PostProcessor *postProcessor;
  };

  int port = 0;
  MHD_Daemon *daemon;

  ServerRoutes getRoutes;
  ServerRoutes putRoutes;
  ServerRoutes postRoutes;
  ServerRoutes deleteRoutes;

  int handleRequest(Connection connection,
                    string url, string method, string version,
                    const char *uploadData, size_t *uploadDataSize,
                    ConnectionInfo **conInfoPtr);

  static int _handleRequest(void *cls, Connection connection,
                            const char *url,
                            const char *method, const char *version,
                            const char *uploadData,
                            size_t *uploadDataSize, void **conCls);

  static void _handleRequestComplete(void *cls, Connection connection,
                                     void **conCls, enum MHD_RequestTerminationCode toe);

  static int _iteratePost(void *coninfoCls,
                          enum MHD_ValueKind kind, const char *key,
                          const char *filename, const char *contentType,
                          const char *transferEncoding, const char *data,
                          uint64_t off, size_t size);

public:
  HttpServer(int port);

  template <class F, class S>
  static RequestHandler bindRequestHandler(F &&f, S *self)
  {
    return std::bind(f, self, std::placeholders::_1, std::placeholders::_2);
  }

  void registerRoute(HttpMethod method, string url, RequestHandler handler);

  int sendResponse(Connection connection, json &j);

  void start();
  void stop();
  void process();
};

#endif // _HTTP_SERVER_H_