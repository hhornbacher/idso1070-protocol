#include "ProtocolWorker.h"
#include "AppWindow.h"

ProtocolWorker::ProtocolWorker() : protocolMutex(), shallStop(false), stopped(false), connected(false), update(false), progress(0)
{
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

ConnectorType ProtocolWorker::getConnectorType()
{
    lock_guard<mutex> lock(protocolMutex);
    Connector *connector = protocol.getConnector();
    if (connector != NULL)
        return connector->getType();
    return (ConnectorType)-1;
}

string ProtocolWorker::getConnectError()
{
    lock_guard<mutex> lock(protocolMutex);
    return protocol.getConnectError();
}

void ProtocolWorker::getDevice(IDSO1070 &dev)
{
    lock_guard<mutex> lock(protocolMutex);
    dev = protocol.getDevice();
}

void ProtocolWorker::connect(string device)
{
    {
        lock_guard<mutex> lock(protocolMutex);
        connecting = true;
        update = true;
    }
    {
        lock_guard<mutex> lock(protocolMutex);
        protocol.connect(device);
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
    {
        lock_guard<mutex> lock(protocolMutex);
        update = true;
    }
}

void ProtocolWorker::disconnect()
{
    lock_guard<mutex> lock(protocolMutex);
    protocol.disconnect();
    update = true;
}

void ProtocolWorker::onUpdateProgress(float progress)
{
    lock_guard<mutex> lock(protocolMutex);
    this->progress = progress;
    update = true;
}

void ProtocolWorker::onInitialized()
{
    lock_guard<mutex> lock(protocolMutex);
    connecting = false;
    update = true;
}