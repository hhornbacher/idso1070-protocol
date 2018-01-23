#include "ControlServer.h"

ControlServer::ControlServer() : protocol(),
                                 httpServer(HttpPort),
                                 channel1StreamServer(protocol.getDevice().getChannel1(), StreamChannel1Port),
                                 channel2StreamServer(protocol.getDevice().getChannel2(), StreamChannel2Port)
{
}

void ControlServer::registerRoutes()
{
    httpServer.registerRoute(METHOD_GET, "/device", HttpServer::bindRequestHandler(&ControlServer::deviceRequestHandler, this));
    httpServer.registerRoute(METHOD_GET, "/trigger", HttpServer::bindRequestHandler(&ControlServer::triggerRequestHandler, this));
    httpServer.registerRoute(METHOD_GET, "/channel/1", HttpServer::bindRequestHandler(&ControlServer::channel1RequestHandler, this));
    httpServer.registerRoute(METHOD_GET, "/channel/2", HttpServer::bindRequestHandler(&ControlServer::channel2RequestHandler, this));

    httpServer.registerRoute(METHOD_GET, "/status", HttpServer::bindRequestHandler(&ControlServer::statusRequestHandler, this));
    httpServer.registerRoute(METHOD_PUT, "/control", HttpServer::bindRequestHandler(&ControlServer::controlRequestHandler, this));

    httpServer.registerRoute(METHOD_GET, "/usb/list", HttpServer::bindRequestHandler(&ControlServer::usbListRequestHandler, this));
    httpServer.registerRoute(METHOD_PUT, "/usb/connect", HttpServer::bindRequestHandler(&ControlServer::usbConnectRequestHandler, this));

    httpServer.registerRoute(METHOD_PUT, "/wifi/connect", HttpServer::bindRequestHandler(&ControlServer::wifiConnectRequestHandler, this));
}

void ControlServer::start()
{
    httpServer.start();
    channel1StreamServer.start();
    channel2StreamServer.start();

    registerRoutes();

    protocol.setProgressHandler(Protocol::bindProgressHandler(&ControlServer::onProgress, this));
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

void ControlServer::statusRequestHandler(HTTPServerRequest &req, HTTPServerResponse &resp)
{
    Object json;
    Connector *connector = protocol.getConnector();
    if (connector)
    {
        bool isConnected = connector->isConnected();
        json.set("connected", isConnected);
        if (isConnected)
        {
            string connectedTo = connector->getType() == CONNECTOR_USB ? "USB" : "WIFI";
            json.set("connectedTo", connectedTo);
        }
        else if (protocol.getConnectError().length() > 0)
        {
            json.set("connectError", protocol.getConnectError());
        }
        json.set("progress", progress * 100);
        json.set("initialized", progress == 1.0f);
        json.set("sampling", protocol.isSampling());
    }
    else
    {
        json.set("connected", false);
    }

    httpServer.sendResponse(req, resp, json);
}

void ControlServer::deviceRequestHandler(HTTPServerRequest &req, HTTPServerResponse &resp)
{
    IDSO1070 &device = protocol.getDevice();

    Object json;
    json.set("timeBase", (int)device.getTimeBase());
    json.set("captureMode", (int)device.getCaptureMode());
    json.set("scopeMode", (int)device.getScopeMode());
    json.set("selectedChannel", device.getSelectedChannelIndex());

    httpServer.sendResponse(req, resp, json);
}

void ControlServer::channel1RequestHandler(HTTPServerRequest &req, HTTPServerResponse &resp)
{
    Channel &channel = protocol.getDevice().getChannel1();
    int port = StreamChannel1Port;

    Object json;
    json.set("enabled", channel.isEnabled());
    json.set("verticalDiv", (int)channel.getVerticalDiv());
    json.set("coupling", (int)channel.getCoupling());
    json.set("verticalPosition", channel.getVerticalPosition());
    json.set("voltage125", channel.getVoltage125());
    json.set("voltageRelay1", channel.getVoltageRL1());
    json.set("voltageRelay2", channel.getVoltageRL2());
    json.set("sampleBufferUsage", ((float)channel.getSampleBuffer().size() / (float)channel.getSampleBuffer().capacity()) * 100.0f);
    json.set("streamPort", port);

    httpServer.sendResponse(req, resp, json);
}

void ControlServer::channel2RequestHandler(HTTPServerRequest &req, HTTPServerResponse &resp)
{
    Channel &channel = protocol.getDevice().getChannel2();
    int port = StreamChannel2Port;
    Object json;
    json.set("enabled", channel.isEnabled());
    json.set("verticalDiv", (int)channel.getVerticalDiv());
    json.set("coupling", (int)channel.getCoupling());
    json.set("verticalPosition", channel.getVerticalPosition());
    json.set("voltage125", channel.getVoltage125());
    json.set("voltageRelay1", channel.getVoltageRL1());
    json.set("voltageRelay2", channel.getVoltageRL2());
    json.set("sampleBufferUsage", ((float)channel.getSampleBuffer().size() / (float)channel.getSampleBuffer().capacity()) * 100.0f);
    json.set("streamPort", port);

    httpServer.sendResponse(req, resp, json);
}

void ControlServer::triggerRequestHandler(HTTPServerRequest &req, HTTPServerResponse &resp)
{
    Trigger &trigger = protocol.getDevice().getTrigger();
    Object json;
    json.set("bottomPWM", trigger.getBottomPWM());
    json.set("topPWM", trigger.getTopPWM());
    json.set("level", trigger.getLevel());
    json.set("channel", (int)trigger.getChannel());
    json.set("slope", (int)trigger.getSlope());
    json.set("mode", (int)trigger.getMode());
    json.set("xPosition", trigger.getXPosition());

    httpServer.sendResponse(req, resp, json);
}

void ControlServer::controlRequestHandler(HTTPServerRequest &req, HTTPServerResponse &resp)
{
    Object json;
    json.set("test", 123);

    httpServer.sendResponse(req, resp, json);
}

void ControlServer::usbListRequestHandler(HTTPServerRequest &req, HTTPServerResponse &resp)
{
    USBConnector::USBDeviceList devices;
    USBConnector::enumerateDevices(devices);

    Object json;
    Array deviceList;

    for (auto device : devices)
    {
        deviceList.add(device.c_str());
    }

    json.set("devices", deviceList);

    httpServer.sendResponse(req, resp, json);
}

void ControlServer::usbConnectRequestHandler(HTTPServerRequest &req, HTTPServerResponse &resp)
{
    Poco::Dynamic::Var jsonRequest;
    httpServer.getJSONBody(req, jsonRequest);
    Object::Ptr jsonRequestObject = jsonRequest.extract<Object::Ptr>();
    Poco::Dynamic::Var device = jsonRequestObject->get("device");

    Object json;
    json.set("connectTo", device.toString());

    protocol.connect(device);
    protocol.init();

    httpServer.sendResponse(req, resp, json);
}

void ControlServer::wifiConnectRequestHandler(HTTPServerRequest &req, HTTPServerResponse &resp)
{
    Poco::Dynamic::Var jsonRequest;
    httpServer.getJSONBody(req, jsonRequest);
    Object::Ptr jsonRequestObject = jsonRequest.extract<Object::Ptr>();
    Poco::Dynamic::Var server = jsonRequestObject->get("server");
    Poco::Dynamic::Var port = jsonRequestObject->get("port");

    string connectTo = server.toString() + ":" + port.toString();

    Object json;
    json.set("connectTo", connectTo);

    int p;
    port.convert(p);
    protocol.connect(server.toString(), p);
    protocol.init();

    httpServer.sendResponse(req, resp, json);
}

void ControlServer::onProgress(float progress)
{
    this->progress = progress;
}