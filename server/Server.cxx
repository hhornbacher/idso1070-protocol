#include "Server.h"

Server::Server() : usbConnection((char *)usbDevice),
                   protocol(usbConnection), cmdFactory(protocol.getDevice())
{
}

void Server::start()
{
    daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, Port, NULL, NULL,
                              &_handleRequest, this, MHD_OPTION_END);
    protocol.start();
    protocol.setProgressHandler(Protocol::bindProgressHandler(&Server::onProgress, this));
    protocol.init();
}

void Server::stop()
{
    MHD_stop_daemon(daemon);
}

void Server::process()
{
    protocol.process();
}

int Server::handleRequest(MHD_Connection *connection, const char *url, const char *method)
{
    if (strcmp(method, "GET") == 0)
    {
        if (strcmp(url, "/status") == 0)
        {
            return statusResponse(connection);
        }
        else if (strcmp(url, "/device") == 0)
        {
            return deviceResponse(connection);
        }
        else if (strcmp(url, "/trigger") == 0)
        {
            return triggerResponse(connection);
        }
        else if (strcmp(url, "/channel/1") == 0)
        {
            return channel1Response(connection);
        }
        else if (strcmp(url, "/channel/2") == 0)
        {
            return channel2Response(connection);
        }
    }
    else if (strcmp(method, "PUT") == 0)
    {
        return MHD_NO;
    }

    return MHD_NO;
}

int Server::sendResponse(MHD_Connection *connection, json &j)
{
    int ret;
    char responseBuffer[ResponseBufferSize];
    MHD_Response *response;
    strncpy(responseBuffer, j.dump(4).c_str(), ResponseBufferSize);
    response = MHD_create_response_from_buffer(strlen(responseBuffer),
                                               (void *)responseBuffer, MHD_RESPMEM_MUST_COPY);

    MHD_add_response_header(response, "Content-Type", "application/json");

    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    return ret;
}

int Server::statusResponse(MHD_Connection *connection)
{
    json j = {
        {"initialized", progress == 1.0f},
        {"progress", progress * 100}};
    return sendResponse(connection, j);
}

int Server::deviceResponse(MHD_Connection *connection)
{
    json j = {
        {"timeBase", (int)protocol.getDevice().getTimeBase()},
        {"captureMode", (int)protocol.getDevice().getCaptureMode()},
        {"scopeMode", (int)protocol.getDevice().getScopeMode()},
        {"selectedChannel", protocol.getDevice().getSelectedChannelIndex()}};

    return sendResponse(connection, j);
}

int Server::channel1Response(MHD_Connection *connection)
{
    json j = {
        {"status", "OK"}};

    return sendResponse(connection, j);
}

int Server::channel2Response(MHD_Connection *connection)
{
    json j = {
        {"status", "OK"}};

    return sendResponse(connection, j);
}

int Server::triggerResponse(MHD_Connection *connection)
{
    json j = {
        {"status", "OK"}};

    return sendResponse(connection, j);
}

void Server::onProgress(float progress)
{
    this->progress = progress;
}

int Server::_handleRequest(void *cls, MHD_Connection *connection,
                           const char *url,
                           const char *method, const char *version,
                           const char *upload_data,
                           size_t *upload_data_size, void **con_cls)
{
    return ((Server *)cls)->handleRequest(connection, url, method);
}