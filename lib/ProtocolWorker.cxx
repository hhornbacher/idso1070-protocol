#include "ProtocolWorker.h"

ProtocolWorker::ProtocolWorker() : shallStop(false),
                                   stopped(true), connected(false),
                                   update(false), progress(0),
                                   connectionLost(false)
{
    Protocol::setConnectionLostHandler(bind(&ProtocolWorker::onConnectionLost, this, placeholders::_1));
    Protocol::setProgressHandler(bind(&ProtocolWorker::onUpdateProgress, this, placeholders::_1));
}

void ProtocolWorker::process()
{
    {
        lock_guard<mutex> lock(stateMutex);
        stopped = false;
    }

    while (!shallStop)
    {
        Protocol::process();
        if (update)
        {
            notifyHandler();
            {
                lock_guard<mutex> lock(stateMutex);
                update = false;
            }
        }
    }

    {
        lock_guard<mutex> lock(stateMutex);
        shallStop = false;
        stopped = true;
    }

    notifyHandler();
}

void ProtocolWorker::startThread()
{
    workerThread = new thread(
        [this] {
            this->process();
        });
}

void ProtocolWorker::stopThread()
{
    {
        lock_guard<mutex> lock(stateMutex);
        shallStop = true;
    }
    while (!stopped)
    {
    }
    if (workerThread->joinable())
        workerThread->join();
}

void ProtocolWorker::connect(string device)
{
    Protocol::connect(device);
    Protocol::init(bind(&ProtocolWorker::onInitialized, this));
    {
        lock_guard<mutex> lock(stateMutex);
        connectionLost = false;
        connectionLostReason = "";
        connecting = true;
        connected = false;
        update = true;
    }
}

void ProtocolWorker::connect(string server, int port)
{
    {
        lock_guard<mutex> lock(stateMutex);
        connectionLost = false;
        connectionLostReason = "";
        connecting = true;
        connected = false;
        update = true;
    }
    Protocol::connect(server, port);
    {
        lock_guard<mutex> lock(stateMutex);
        update = true;
    }
    Protocol::init(bind(&ProtocolWorker::onInitialized, this));
}

void ProtocolWorker::disconnect()
{
    Protocol::disconnect();
    {
        lock_guard<mutex> lock(stateMutex);
        connecting = false;
        connected = false;
        update = true;
    }
}

bool ProtocolWorker::isConnected() const
{
    lock_guard<mutex> lock(stateMutex);
    return connected;
}

bool ProtocolWorker::isConnecting() const
{
    lock_guard<mutex> lock(stateMutex);
    return connecting;
}

bool ProtocolWorker::isConnectionLost() const
{
    lock_guard<mutex> lock(stateMutex);
    return connectionLost;
}

ConnectorType ProtocolWorker::getConnectorType()
{
    Connector *connector = Protocol::getConnector();
    if (connector != NULL)
        return connector->getType();
    return (ConnectorType)-1;
}

string ProtocolWorker::getConnectionLostReason() const
{
    lock_guard<mutex> lock(stateMutex);
    return connectionLostReason;
}

void ProtocolWorker::onInitialized()
{
    lock_guard<mutex> lock(stateMutex);
    connecting = false;
    connected = true;
    update = true;
}

void ProtocolWorker::onConnectionLost(Connector::Exception &e)
{
    lock_guard<mutex> lock(stateMutex);
    connectionLostReason = e.what();
    connectionLost = true;
    connected = false;
    connecting = false;
    update = true;
}

void ProtocolWorker::onUpdateProgress(float progress)
{
    lock_guard<mutex> lock(stateMutex);
    this->progress = progress;
    update = true;
}

void ProtocolWorker::onUpdateUI()
{
    lock_guard<mutex> lock(stateMutex);
    update = true;
}

bool ProtocolWorker::hasStopped() const
{
    lock_guard<mutex> lock(stateMutex);
    return stopped;
}

float ProtocolWorker::getProgress() const
{
    lock_guard<mutex> lock(stateMutex);
    return progress;
}

void ProtocolWorker::setNotifyHandler(NotifyHandler notifyHandler)
{
    this->notifyHandler = notifyHandler;
}

void ProtocolWorker::fetchDevice(IDSO1070 &dev)
{
    Protocol::fetchDevice(dev);
}

void ProtocolWorker::fetchSamples(SampleBuffer &buffer)
{
    Protocol::fetchSamples(buffer);
}

void ProtocolWorker::fetchTransmissionLog(Protocol::TransmissionLog &log)
{
    Protocol::fetchTransmissionLog(log);
    Protocol::clearTransmissionLog(false);
}

void ProtocolWorker::startSampling()
{
    Protocol::startSampling(bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::stopSampling()
{
    Protocol::stopSampling(bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::readBatteryLevel()
{
    Protocol::readBatteryLevel(bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::setTimeBase(TimeBase timeBase)
{
    Protocol::setTimeBase(timeBase, bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::setScopeMode(ScopeMode scopeMode)
{
    Protocol::setScopeMode(scopeMode, bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::setCaptureMode(CaptureMode captureMode)
{
    Protocol::setCaptureMode(captureMode, bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::enableChannel(ChannelSelector channel)
{
    Protocol::enableChannel(channel, bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::disableChannel(ChannelSelector channel)
{
    Protocol::disableChannel(channel, bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::setChannelVerticalDiv(ChannelSelector channel, VoltageDiv div)
{
    Protocol::setChannelVerticalDiv(channel, div, bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::setChannelCoupling(ChannelSelector channel, InputCoupling coupling)
{
    Protocol::setChannelCoupling(channel, coupling, bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::setChannelVerticalPosition(ChannelSelector channel, uint16_t pos)
{
    Protocol::setChannelVerticalPosition(channel, pos, bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::setTriggerMode(TriggerMode mode)
{
    Protocol::setTriggerMode(mode, bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::setTriggerChannel(TriggerChannel channel)
{
    Protocol::setTriggerChannel(channel, bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::setTriggerSlope(TriggerSlope slope)
{
    Protocol::setTriggerSlope(slope, bind(&ProtocolWorker::onUpdateUI, this));
}

void ProtocolWorker::setTriggerLevel(uint16_t level)
{
    Protocol::setTriggerLevel(level, bind(&ProtocolWorker::onUpdateUI, this));
}