#include "ControlServer.h"

ControlServer::ControlServer() : usbConnection((char *)usbDevice),
                                 protocol(usbConnection), cmdFactory(protocol.getDevice()),
                                 channel1StreamServer(protocol.getDevice().getChannel1(), StreamChannel1Port),
                                 channel2StreamServer(protocol.getDevice().getChannel2(), StreamChannel2Port)
{
}

void ControlServer::start()
{
    daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, HttpPort, NULL, NULL,
                              &_handleRequest, this, MHD_OPTION_END);
    channel1StreamServer.start();
    channel2StreamServer.start();

    protocol.start();
    protocol.setProgressHandler(Protocol::bindProgressHandler(&ControlServer::onProgress, this));
    protocol.init();
}

void ControlServer::stop()
{
    MHD_stop_daemon(daemon);
    channel1StreamServer.stop();
    channel2StreamServer.stop();
}

void ControlServer::process()
{
    protocol.process();
    channel1StreamServer.process();
    channel2StreamServer.process();
}

int ControlServer::handleRequest(MHD_Connection *connection, const char *url, const char *method)
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

int ControlServer::sendResponse(MHD_Connection *connection, json &j)
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

int ControlServer::statusResponse(MHD_Connection *connection)
{
    json j = {
        {"initialized", progress == 1.0f},
        {"progress", progress * 100},
        {"sampling", protocol.isSampling()}};
    return sendResponse(connection, j);
}

int ControlServer::deviceResponse(MHD_Connection *connection)
{
    IDSO1070A &device = protocol.getDevice();
    json j = {
        {"timeBase", (int)device.getTimeBase()},
        {"captureMode", (int)device.getCaptureMode()},
        {"scopeMode", (int)device.getScopeMode()},
        {"selectedChannel", device.getSelectedChannelIndex()}};

    return sendResponse(connection, j);
}

int ControlServer::channel1Response(MHD_Connection *connection)
{
    Channel &channel = protocol.getDevice().getChannel1();
    json j = {
        {"enabled", channel.isEnabled()},
        {"verticalDiv", (int)channel.getVerticalDiv()},
        {"coupling", (int)channel.getCoupling()},
        {"verticalPosition", channel.getVerticalPosition()},
        {"voltage125", channel.getVoltage125()},
        {"voltageRelay1", channel.getVoltageRL1()},
        {"voltageRelay2", channel.getVoltageRL2()},
        {"sampleBufferSize", channel.getSampleBuffer().size()}};

    return sendResponse(connection, j);
}

int ControlServer::channel2Response(MHD_Connection *connection)
{
    Channel &channel = protocol.getDevice().getChannel2();
    json j = {
        {"enabled", channel.isEnabled()},
        {"verticalDiv", (int)channel.getVerticalDiv()},
        {"coupling", (int)channel.getCoupling()},
        {"verticalPosition", channel.getVerticalPosition()},
        {"voltage125", channel.getVoltage125()},
        {"voltageRelay1", channel.getVoltageRL1()},
        {"voltageRelay2", channel.getVoltageRL2()},
        {"sampleBufferSize", channel.getSampleBuffer().size()}};

    return sendResponse(connection, j);
}

int ControlServer::triggerResponse(MHD_Connection *connection)
{
    Trigger &trigger = protocol.getDevice().getTrigger();
    json j = {
        {"bottomPWM", trigger.getBottomPWM()},
        {"topPWM", trigger.getTopPWM()},
        {"level", trigger.getLevel()},
        {"channel", (int)trigger.getChannel()},
        {"slope", (int)trigger.getSlope()},
        {"mode", (int)trigger.getMode()},
        {"xPosition", trigger.getXPosition()}};

    return sendResponse(connection, j);
}

void ControlServer::onProgress(float progress)
{
    this->progress = progress;
}

int ControlServer::_handleRequest(void *cls, MHD_Connection *connection,
                                  const char *url,
                                  const char *method, const char *version,
                                  const char *upload_data,
                                  size_t *upload_data_size, void **con_cls)
{
    return ((ControlServer *)cls)->handleRequest(connection, url, method);
}