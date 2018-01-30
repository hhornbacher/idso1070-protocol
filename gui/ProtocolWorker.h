#ifndef _PROTOCOL_WORKER_H_
#define _PROTOCOL_WORKER_H_

#include <Protocol.h>

#include <gtkmm.h>
#include <thread>
#include <mutex>

class AppWindow;

class ProtocolWorker
{
public:
  ProtocolWorker();

  // Thread main loop
  void process(AppWindow *caller);

  // Thread execution control & status
  void stop();
  bool hasStopped() const;
  float getProgress() const;

  // Connection status
  bool isConnected() const;
  bool isConnecting() const;
  bool isConnectionLost() const;
  bool isSampling();
  ConnectorType getConnectorType();
  string getConnectionLostReason() const;

  // Connection control
  void connect(string device);
  void connect(string server, int port);
  void disconnect();

  // Sampling control
  void startSampling();
  void stopSampling();

  // In-Thread callbacks
  void onConnectionLost(ConnectionException &e);
  void onUpdateProgress(float progress);
  void onInitialized();
  void onSamplingStarted();
  void onSamplingStopped();

  // Get copy of current device state
  void getDevice(IDSO1070 &dev);

protected:
  // Thread sync mutex
  mutable mutex protocolMutex;

  // Protocol itself
  Protocol protocol;

  // Thread execution control & status members
  bool shallStop;
  bool stopped;

  // Connection status members
  string connectionLostReason;
  bool connectionLost;
  bool connected;
  bool connecting;
  float progress;

  // Notify UI to update
  bool update;
};

#endif // _PROTOCOL_WORKER_H_