#include "HttpServer.h"

HttpServer::HttpServer(int port) : _server(this, ServerSocket(port), new HTTPServerParams)
{
    notFoundHandler = bindRequestHandler(&HttpServer::sendErrorNotFound, this);
}

void HttpServer::start()
{
    _server.start();
}

void HttpServer::stop()
{
    _server.stop();
}

void HttpServer::process()
{
}

void HttpServer::sendErrorNotFound(HTTPServerRequest &req, HTTPServerResponse &resp)
{
    resp.setStatus(HTTPResponse::HTTP_NOT_FOUND);
    resp.setContentType("application/json");

    Object json;
    json.set("error", 404);
    json.set("msg", "Not found!");
    ostream &out = resp.send();
    json.stringify(out, 4);
    out.flush();
}

void HttpServer::sendResponse(HTTPServerRequest &req, HTTPServerResponse &resp, Object &json)
{
    resp.setStatus(HTTPResponse::HTTP_OK);
    resp.setContentType("application/json");

    try
    {
        // Add access control allow headers
        resp.set("Access-Control-Allow-Origin", req.get("Origin"));
        resp.set("Access-Control-Allow-Headers", req.get("Access-Control-Request-Headers"));
        resp.set("Access-Control-Allow-Methods", "OPTIONS, GET, PUT");
    }
    catch (Poco::NotFoundException e)
    {
    }

    ostream &out = resp.send();
    json.stringify(out, 4);
    out.flush();
}

void HttpServer::registerRoute(HttpMethod method, string url, RequestHandler handler)
{
    switch (method)
    {
    case METHOD_GET:
        getRoutes[url] = handler;
        break;
    case METHOD_POST:
        postRoutes[url] = handler;
        break;
    case METHOD_PUT:
        putRoutes[url] = handler;
        break;
    case METHOD_DELETE:
        deleteRoutes[url] = handler;
        break;
    }
}

HTTPRequestHandler *HttpServer::createRequestHandler(const HTTPServerRequest &req)
{

    string method = req.getMethod();
    string url = req.getURI();
    // CORS support
    if (method == "OPTIONS")
    {
        try
        {
            method = req.get("Access-Control-Request-Method");
        }
        catch (Poco::NotFoundException e)
        {
        }
    }

    if (method == "GET")
    {
        try
        {
            return new HttpRequestHandler(getRoutes.at(url));
        }
        catch (out_of_range e)
        {
            return new HttpRequestHandler(notFoundHandler);
        }
    }
    else if (method == "POST")
    {
        try
        {
            return new HttpRequestHandler(postRoutes.at(url));
        }
        catch (out_of_range e)
        {
            return new HttpRequestHandler(notFoundHandler);
        }
    }
    else if (method == "PUT")
    {
        try
        {
            return new HttpRequestHandler(putRoutes.at(url));
        }
        catch (out_of_range e)
        {
            return new HttpRequestHandler(notFoundHandler);
        }
    }
    else if (method == "DELETE")
    {
        try
        {
            return new HttpRequestHandler(deleteRoutes.at(url));
        }
        catch (out_of_range e)
        {
            return new HttpRequestHandler(notFoundHandler);
        }
    }
    return new HttpRequestHandler(notFoundHandler);
}
