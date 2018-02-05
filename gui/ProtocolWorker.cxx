#include "ProtocolWorker.h"
#include "AppWindow.h"

ProtocolWorker::ProtocolWorker() : protocolMutex(), shallStop(false),
                                   stopped(false), connected(false),
                                   update(false), progress(0),
                                   connectionLost(false)
{
    protocol.setConnectionLostHandler(bind(&ProtocolWorker::onConnectionLost, this, placeholders::_1));
    protocol.setProgressHandler(bind(&ProtocolWorker::onUpdateProgress, this, placeholders::_1));
}

void ProtocolWorker::process(AppWindow *caller)
{
    {
        lock_guard<mutex> lock(protocolMutex);
        stopped = false;
    }

    while (!shallStop)
    {
        protocol.process();
        if (update)
        {
            caller->notify();
            {
                lock_guard<mutex> lock(protocolMutex);
                update = false;
            }
        }
    }

    {
        lock_guard<mutex> lock(protocolMutex);
        shallStop = false;
        stopped = true;
    }

    caller->notify();
}

void ProtocolWorker::stop()
{
    lock_guard<mutex> lock(protocolMutex);
    shallStop = true;
}

void ProtocolWorker::connect(string device)
{
    {
        lock_guard<mutex> lock(protocolMutex);
        connectionLost = false;
        connectionLostReason = "";
        connecting = true;
        connected = false;
        update = true;
    }
    protocol.connect(device);
    {
        lock_guard<mutex> lock(protocolMutex);
        protocol.init(bind(&ProtocolWorker::onInitialized, this));
    }
}

void ProtocolWorker::connect(string server, int port)
{
    {
        lock_guard<mutex> lock(protocolMutex);
        connectionLost = false;
        connectionLostReason = "";
        connecting = true;
        connected = false;
        update = true;
    }
    protocol.connect(server, port);
    {
        lock_guard<mutex> lock(protocolMutex);
        Connector *connector = protocol.getConnector();
        update = true;
    }
    Command::ResponseHandler initializedHandler = bind(&ProtocolWorker::onInitialized, this);
    {
        lock_guard<mutex> lock(protocolMutex);
        protocol.init(initializedHandler);
    }
}

void ProtocolWorker::disconnect()
{
    lock_guard<mutex> lock(protocolMutex);
    protocol.disconnect();
    connecting = false;
    connected = false;
    update = true;
}

void ProtocolWorker::startSampling()
{
    lock_guard<mutex> lock(protocolMutex);
    protocol.startSampling(bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::stopSampling()
{
    lock_guard<mutex> lock(protocolMutex);
    protocol.stopSampling(bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::readBatteryLevel()
{
    lock_guard<mutex> lock(protocolMutex);
    protocol.readBatteryLevel(bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::setTimeBase(TimeBase timeBase)
{
    lock_guard<mutex> lock(protocolMutex);
    protocol.setTimeBase(timeBase, bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::setScopeMode(ScopeMode scopeMode)
{
    lock_guard<mutex> lock(protocolMutex);
    protocol.setScopeMode(scopeMode, bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::setCaptureMode(CaptureMode captureMode)
{
    lock_guard<mutex> lock(protocolMutex);
    protocol.setCaptureMode(captureMode, bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::enableChannel(ChannelSelector channel)
{
    lock_guard<mutex> lock(protocolMutex);
    protocol.enableChannel(channel, bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::disableChannel(ChannelSelector channel)
{
    lock_guard<mutex> lock(protocolMutex);
    protocol.disableChannel(channel, bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::setChannelVerticalDiv(ChannelSelector channel, VoltageDiv div)
{
    lock_guard<mutex> lock(protocolMutex);
    protocol.setChannelVerticalDiv(channel, div, bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::setChannelCoupling(ChannelSelector channel, InputCoupling coupling)
{
    lock_guard<mutex> lock(protocolMutex);
    protocol.setChannelCoupling(channel, coupling, bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::setChannelVerticalPosition(ChannelSelector channel, uint16_t pos)
{
    lock_guard<mutex> lock(protocolMutex);
    protocol.setChannelVerticalPosition(channel, pos, bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::setTriggerMode(TriggerMode mode)
{
    lock_guard<mutex> lock(protocolMutex);
    protocol.setTriggerMode(mode, bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::setTriggerChannel(TriggerChannel channel)
{
    lock_guard<mutex> lock(protocolMutex);
    protocol.setTriggerChannel(channel, bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::setTriggerSlope(TriggerSlope slope)
{
    lock_guard<mutex> lock(protocolMutex);
    protocol.setTriggerSlope(slope, bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::setTriggerLevel(uint16_t level)
{
    lock_guard<mutex> lock(protocolMutex);
    protocol.setTriggerLevel(level, bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::onConnectionLost(Connector::Exception &e)
{
    lock_guard<mutex> lock(protocolMutex);
    connectionLostReason = e.what();
    connectionLost = true;
    connected = false;
    connecting = false;
    update = true;
}

void ProtocolWorker::onUpdateUI()
{
    lock_guard<mutex> lock(protocolMutex);
    update = true;
}

void ProtocolWorker::onUpdateProgress(float progress)
{
    this->progress = progress;
    {
        lock_guard<mutex> lock(protocolMutex);
        update = true;
    }
}

void ProtocolWorker::onInitialized()
{
    lock_guard<mutex> lock(protocolMutex);
    connecting = false;
    connected = true;
    update = true;
}

bool ProtocolWorker::hasStopped() const
{
    lock_guard<mutex> lock(protocolMutex);
    return stopped;
}

float ProtocolWorker::getProgress() const
{
    lock_guard<mutex> lock(protocolMutex);
    return progress;
}

bool ProtocolWorker::isConnected() const
{
    lock_guard<mutex> lock(protocolMutex);
    return connected;
}

bool ProtocolWorker::isConnecting() const
{
    lock_guard<mutex> lock(protocolMutex);
    return connecting;
}

bool ProtocolWorker::isConnectionLost() const
{
    lock_guard<mutex> lock(protocolMutex);
    return connectionLost;
}

ConnectorType ProtocolWorker::getConnectorType()
{
    lock_guard<mutex> lock(protocolMutex);
    Connector *connector = protocol.getConnector();
    if (connector != NULL)
        return connector->getType();
    return (ConnectorType)-1;
}

string ProtocolWorker::getConnectionLostReason() const
{
    lock_guard<mutex> lock(protocolMutex);
    return connectionLostReason;
}

void ProtocolWorker::getDevice(IDSO1070 &dev)
{
    lock_guard<mutex> lock(protocolMutex);
    dev = protocol.getDevice();
}

void ProtocolWorker::fetchChannelSamples(ChannelSelector channel, Sample::SampleBuffer &buffer)
{

    lock_guard<mutex> lock(protocolMutex);
    protocol.fetchChannelSamples(channel, buffer);
}

void ProtocolWorker::getTransmissionLog(Protocol::TransmissionLog &log)
{
    lock_guard<mutex> lock(protocolMutex);
    log = protocol.getTransmissionLog();
    protocol.clearTransmissionLog(false);
}