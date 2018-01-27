#ifndef _PROTOCOL_WORKER_H_
#define _PROTOCOL_WORKER_H_

#include <Protocol.h>

#include <gtkmm.h>
#include <thread>
#include <mutex>

using namespace std;

class AppWindow;

class ProtocolWorker
{
  public:
    ProtocolWorker();

    void process(AppWindow *caller);
    void stop();
    bool hasStopped() const;
    float getProgress() const;

    void connect(string device);

    void onUpdateProgress(float progress);

  protected:
    mutable mutex protocolMutex;

    Protocol protocol;

    bool shallStop;
    bool stopped;
    bool update;
    float progress;
};

#endif // _PROTOCOL_WORKER_H_