#include "ProtocolWorker.h"
#include "AppWindow.h"

ProtocolWorker::ProtocolWorker() : protocolMutex(), shallStop(false),
                                   stopped(false), connected(false),
                                   update(false), progress(0),
                                   connectionLost(false)
{
    protocol.setConnectionLostHandler(bind(&ProtocolWorker::onConnectionLost, this, placeholders::_1));
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
        Connector *connector = protocol.getConnector();
        connected = (connector && connector->isConnected());
        update = true;
    }
    if (connected)
    {
        Protocol::ProgressHandler progressHandler = bind(&ProtocolWorker::onUpdateProgress, this, placeholders::_1);
        Protocol::BatchFinishedHandler initializedHandler = bind(&ProtocolWorker::onInitialized, this);
        {
            lock_guard<mutex> lock(protocolMutex);
            protocol.init(progressHandler, initializedHandler);
        }
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
        connected = (connector && connector->isConnected());
        update = true;
    }
    if (connected)
    {
        Protocol::ProgressHandler progressHandler = bind(&ProtocolWorker::onUpdateProgress, this, placeholders::_1);
        Protocol::BatchFinishedHandler initializedHandler = bind(&ProtocolWorker::onInitialized, this);
        {
            lock_guard<mutex> lock(protocolMutex);
            protocol.init(progressHandler, initializedHandler);
        }
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
    protocol.startSampling(bind(&ProtocolWorker::onSamplingStarted, this));
}

void ProtocolWorker::stopSampling()
{
    lock_guard<mutex> lock(protocolMutex);
    protocol.stopSampling(bind(&ProtocolWorker::onSamplingStopped, this));
}

void ProtocolWorker::onConnectionLost(ConnectionException &e)
{
    lock_guard<mutex> lock(protocolMutex);
    connectionLostReason = e.what();
    connectionLost = true;
    connected = false;
    connecting = false;
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
    update = true;
}

void ProtocolWorker::onSamplingStarted()
{
    lock_guard<mutex> lock(protocolMutex);
    // connecting = false;
    update = true;
}

void ProtocolWorker::onSamplingStopped()
{
    lock_guard<mutex> lock(protocolMutex);
    // connecting = false;
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

bool ProtocolWorker::isSampling()
{
    lock_guard<mutex> lock(protocolMutex);
    return protocol.isSampling();
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