#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_

#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <unistd.h>

#include <Poco/Exception.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Parser.h>

#include <iostream>
#include <functional>

using namespace Poco::JSON;
using namespace Poco::Net;
using namespace std;

enum HttpMethod
{
  METHOD_GET,
  METHOD_POST,
  METHOD_PUT,
  METHOD_DELETE
};

class HttpServer : public HTTPRequestHandlerFactory
{
public:
  static const int ResponseBufferSize = 1024 * 16;
  static const int PostBufferSize = 1024 * 16;

  typedef function<void(HTTPServerRequest &, HTTPServerResponse &)> RequestHandler;
  typedef map<string, RequestHandler> ServerRoutes;

private:
  int port = 0;

  ServerRoutes getRoutes;
  ServerRoutes putRoutes;
  ServerRoutes postRoutes;
  ServerRoutes deleteRoutes;

  HTTPServer _server;

  RequestHandler notFoundHandler;

  class HttpRequestHandler : public HTTPRequestHandler
  {
  private:
    RequestHandler handler;

  public:
    HttpRequestHandler(RequestHandler handler) : handler(handler)
    {
    }

    void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
    {
      handler(req, resp);
    }
  };
  void sendErrorNotFound(HTTPServerRequest &req, HTTPServerResponse &resp);

public:
  HttpServer(int port);

  template <class F, class S>
  static RequestHandler bindRequestHandler(F &&f, S *self)
  {
    return std::bind(f, self, std::placeholders::_1, std::placeholders::_2);
  }

  void registerRoute(HttpMethod method, string url, RequestHandler handler);

  void sendResponse(HTTPServerRequest &req, HTTPServerResponse &resp, Object &json);
  void getJSONBody(HTTPServerRequest &req, Poco::Dynamic::Var &json);

  HTTPRequestHandler *createRequestHandler(const HTTPServerRequest &req);

  void start();
  void stop();
  void process();
};

#endif // _HTTP_SERVER_H_