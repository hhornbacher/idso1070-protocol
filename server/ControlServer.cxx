#include "ControlServer.h"

ControlServer::ControlServer() : usbConnection((char *)usbDevice),
                                 protocol(usbConnection), cmdFactory(protocol.getDevice()),
                                 httpServer(HttpPort),
                                 channel1StreamServer(protocol.getDevice().getChannel1(), StreamChannel1Port),
                                 channel2StreamServer(protocol.getDevice().getChannel2(), StreamChannel2Port)
{
}

void ControlServer::start()
{
    httpServer.start();
    channel1StreamServer.start();
    channel2StreamServer.start();

    httpServer.registerRoute(METHOD_GET, "/status", HttpServer::bindRequestHandler(&ControlServer::statusRequestHandler, this));
    httpServer.registerRoute(METHOD_GET, "/device", HttpServer::bindRequestHandler(&ControlServer::deviceRequestHandler, this));
    httpServer.registerRoute(METHOD_GET, "/trigger", HttpServer::bindRequestHandler(&ControlServer::triggerRequestHandler, this));
    httpServer.registerRoute(METHOD_GET, "/channel/1", HttpServer::bindRequestHandler(&ControlServer::channel1RequestHandler, this));
    httpServer.registerRoute(METHOD_GET, "/channel/2", HttpServer::bindRequestHandler(&ControlServer::channel2RequestHandler, this));
    httpServer.registerRoute(METHOD_PUT, "/control", HttpServer::bindRequestHandler(&ControlServer::controlRequestHandler, this));

    protocol.start();
    protocol.setProgressHandler(Protocol::bindProgressHandler(&ControlServer::onProgress, this));
    protocol.init();
    protocol.startSampling();
}

void ControlServer::stop()
{
    httpServer.stop();
    channel1StreamServer.stop();
    channel2StreamServer.stop();
}

void ControlServer::process()
{
    protocol.process();
    httpServer.process();
    channel1StreamServer.process();
    channel2StreamServer.process();
}

int ControlServer::statusRequestHandler(HttpServer::Connection connection, json *)
{
    json j = {
        {"initialized", progress == 1.0f},
        {"progress", progress * 100},
        {"sampling", protocol.isSampling()}};
    return httpServer.sendResponse(connection, j);
}

int ControlServer::deviceRequestHandler(HttpServer::Connection connection, json *)
{
    IDSO1070 &device = protocol.getDevice();
    json j = {
        {"timeBase", (int)device.getTimeBase()},
        {"captureMode", (int)device.getCaptureMode()},
        {"scopeMode", (int)device.getScopeMode()},
        {"selectedChannel", device.getSelectedChannelIndex()}};

    return httpServer.sendResponse(connection, j);
}

int ControlServer::channel1RequestHandler(HttpServer::Connection connection, json *)
{
    Channel &channel = protocol.getDevice().getChannel1();
    int port = StreamChannel1Port;
    json j = {
        {"enabled", channel.isEnabled()},
        {"verticalDiv", (int)channel.getVerticalDiv()},
        {"coupling", (int)channel.getCoupling()},
        {"verticalPosition", channel.getVerticalPosition()},
        {"voltage125", channel.getVoltage125()},
        {"voltageRelay1", channel.getVoltageRL1()},
        {"voltageRelay2", channel.getVoltageRL2()},
        {"sampleBufferUsage", ((float)channel.getSampleBuffer().size() / (float)channel.getSampleBuffer().capacity()) * 100.0f},
        {"streamPort", port}};

    return httpServer.sendResponse(connection, j);
}

int ControlServer::channel2RequestHandler(HttpServer::Connection connection, json *)
{
    Channel &channel = protocol.getDevice().getChannel2();
    int port = StreamChannel2Port;
    json j = {
        {"enabled", channel.isEnabled()},
        {"verticalDiv", (int)channel.getVerticalDiv()},
        {"coupling", (int)channel.getCoupling()},
        {"verticalPosition", channel.getVerticalPosition()},
        {"voltage125", channel.getVoltage125()},
        {"voltageRelay1", channel.getVoltageRL1()},
        {"voltageRelay2", channel.getVoltageRL2()},
        {"sampleBufferUsage", ((float)channel.getSampleBuffer().size() / (float)channel.getSampleBuffer().capacity()) * 100.0f},
        {"streamPort", port}};

    return httpServer.sendResponse(connection, j);
}

int ControlServer::triggerRequestHandler(HttpServer::Connection connection, json *)
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

    return ControlServer::httpServer.sendResponse(connection, j);
}

int ControlServer::controlRequestHandler(HttpServer::Connection connection, json *)
{
    Trigger &trigger = protocol.getDevice().getTrigger();
    json j = {};

    return ControlServer::httpServer.sendResponse(connection, j);
}

void ControlServer::onProgress(float progress)
{
    this->progress = progress;
}