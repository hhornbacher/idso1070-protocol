#include "ProtocolWorker.h"
#include "AppWindow.h"

ProtocolWorker::ProtocolWorker() : shallStop(false), stopped(false), update(false), progress(0)
{
    protocol.setProgressHandler(this, &ProtocolWorker::onUpdateProgress);
}

void ProtocolWorker::process(AppWindow *caller)
{
    {
        lock_guard<mutex> lock(mutex);
        stopped = false;
    }

    while (!shallStop)
    {
        {
            lock_guard<mutex> lock(mutex);
            protocol.process();
        }
        if (update)
        {
            caller->notify();
            update = false;
        }
    }

    {
        lock_guard<mutex> lock(mutex);
        shallStop = false;
        stopped = true;
    }

    caller->notify();
}

void ProtocolWorker::stop()
{
    lock_guard<mutex> lock(mutex);
    shallStop = true;
}

bool ProtocolWorker::hasStopped() const
{
    lock_guard<mutex> lock(mutex);
    return stopped;
}
float ProtocolWorker::getProgress() const
{
    lock_guard<mutex> lock(mutex);
    return progress;
}

void ProtocolWorker::connect(string device)
{
    lock_guard<mutex> lock(mutex);
    protocol.connect(device);
    protocol.init();
    update = true;
}

void ProtocolWorker::onUpdateProgress(float progress)
{
    this->progress = progress;
    update = true;
}