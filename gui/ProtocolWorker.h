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
  ConnectorType getConnectorType();
  string getConnectionLostReason() const;

  // Connection control
  void connect(string device);
  void connect(string server, int port);
  void disconnect();

  // Sampling control
  void startSampling();
  void stopSampling();

  // Device control
  void init();
  void readBatteryLevel();
  void setTimeBase(TimeBase timeBase);
  void setScopeMode(ScopeMode scopeMode);
  void setCaptureMode(CaptureMode captureMode);

  // Channel control
  void enableChannel(ChannelSelector channel);
  void disableChannel(ChannelSelector channel);
  void setChannelVerticalDiv(ChannelSelector channel, VoltageDiv div);
  void setChannelCoupling(ChannelSelector channel, InputCoupling coupling);
  void setChannelVerticalPosition(ChannelSelector channel, uint16_t pos);

  // Trigger control
  void setTriggerMode(TriggerMode mode);
  void setTriggerChannel(TriggerChannel channel);
  void setTriggerSlope(TriggerSlope slope);
  void setTriggerLevel(uint16_t level);

  // In-Thread callbacks
  void onConnectionLost(Connector::Exception &e);
  void onUpdateUI();
  void onUpdateProgress(float progress);
  void onInitialized();

  // Get copy of current device state
  void getDevice(IDSO1070 &dev);

  void fetchSamples(Sample::SampleBuffer &buffer);

  // Get copy of the transmission log
  void getTransmissionLog(Protocol::TransmissionLog &log);

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