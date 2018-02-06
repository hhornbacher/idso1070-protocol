#ifndef _IDSO1070_H_
#define _IDSO1070_H_

#include "base.h"

class Protocol;
class PacketParser;

class IDSO1070
{
  friend class Protocol;
  friend class PacketParser;

public:
  static const int MaxPWM = 4095;
  static const int MemoryDepth = 2000;
  static const int SamplesCountPerPacket = 500;
  static const int MaxSample = 248;
  static const int DeviceMemoryDepth = 6 * 1024;

  struct DeviceSettings
  {
    // freqDiv and timeBase are related to each other!
    TimeBase timeBase;
    uint32_t freqDiv;

    ScopeMode scopeMode;
    CaptureMode captureMode;

    uint8_t batteryLevel;

    string armFirmwareVersion;
    string fpgaFirmwareVersion;
    string productName;
    string userName;

    uint8_t diffFixData[2][256];
    uint16_t caliLevel[100];
    uint8_t fpgaAlert[40];
  };
  struct ChannelSettings
  {
    bool enabled;
    VoltageDiv verticalDiv;
    InputCoupling coupling;
    int8_t verticalPosition;
    uint16_t pwmArray[9][2];

    double voltage125;
    double voltageRL1;
    double voltageRL2;
  };
  struct TriggerSettings
  {
    bool isHold = false;
    TriggerMode mode;
    TriggerChannel channel;
    TriggerSlope slope;
    uint16_t level;
    double xPosition;
    uint16_t innerPWM[4];
    uint16_t outerPWM[2];
  };

  IDSO1070();

  // Copy assignment operator
  IDSO1070 &operator=(IDSO1070 obj);

  // General device settings
  TimeBase getTimeBase();
  CaptureMode getCaptureMode();
  ScopeMode getScopeMode();
  string getARMFirmwareVersion();
  string getFPGAFirmwareVersion();
  string getProductName();
  string getUserName();
  uint16_t *getCaliLevel();
  uint8_t *getFPGAAlert();
  uint8_t getBatteryLevel();
  uint32_t getFreqDiv();

  // Channel data access methods
  ChannelSelector getSelectedChannel();
  bool isChannelEnabled(ChannelSelector channel);
  VoltageDiv getChannelVerticalDiv(ChannelSelector channel);
  InputCoupling getChannelCoupling(ChannelSelector channel);
  int8_t getChannelVerticalPosition(ChannelSelector channel);
  uint16_t getChannelPWM(ChannelSelector channel, uint8_t a, uint8_t b);
  uint16_t *getChannelPWM(ChannelSelector channel, uint8_t a);
  double getChannelVoltage125(ChannelSelector channel);
  double getChannelVoltageRL1(ChannelSelector channel);
  double getChannelVoltageRL2(ChannelSelector channel);

  // Trigger data access methods
  uint16_t getTriggerLevel();
  TriggerChannel getTriggerChannel();
  TriggerSlope getTriggerSlope();
  TriggerMode getTriggerMode();
  double getTriggerXPosition();
  uint16_t getTriggerInnerPWM(uint8_t index);
  uint16_t *getTriggerInnerPWM();
  uint16_t getTriggerOuterPWM(uint8_t index);
  uint16_t *getTriggerOuterPWM();
  uint16_t getTriggerBottomPWM();
  uint16_t getTriggerTopPWM();

  // Misc
  bool isSampling();
  int getLittlePacketStatus();
  bool isSampleRate200Mor250M();
  TimeBase getTimeBaseFromFreqDiv();
  size_t getSamplesNumberOfOneFrame();
  uint8_t getEnabledChannelsCount();
  uint8_t getPacketsNumber();

protected:
  // All setters are protected!
  void setTimeBase(TimeBase timeBase);
  void setCaptureMode(CaptureMode captureMode);
  void setScopeMode(ScopeMode scopeMode);
  void setARMFirmwareVersion(string version);
  void setFPGAFirmwareVersion(string version);
  void setProductName(string productName);
  void setUserName(string userName);
  void setCaliLevel(uint8_t *data);
  void setFPGAAlert(uint8_t *data);
  void setDiffFixData(size_t channel, size_t offset, uint8_t *data);
  void setBatteryLevel(uint8_t batteryLevel);
  void setFreqDiv(uint32_t freqDiv);
  void setSelectedChannel(ChannelSelector channel);
  void enableChannel(ChannelSelector channel);
  void disableChannel(ChannelSelector channel);
  void setChannelVerticalDiv(ChannelSelector channel, VoltageDiv verticalDiv);
  void setChannelCoupling(ChannelSelector channel, InputCoupling coupling);
  void setChannelVerticalPosition(ChannelSelector channel, int16_t verticalPosition);
  void setChannelPWM(ChannelSelector channel, uint16_t pwm, uint8_t a, uint8_t b);
  void setChannelVoltage125(ChannelSelector channel, double voltage);
  void setChannelVoltageRL1(ChannelSelector channel, double voltage);
  void setChannelVoltageRL2(ChannelSelector channel, double voltage);
  void setTriggerLevel(uint16_t i);
  void setTriggerChannel(TriggerChannel channel);
  void setTriggerSlope(TriggerSlope slope);
  void setTriggerMode(TriggerMode mode);
  void setTriggerXPosition(double xPosition);
  void setTriggerInnerPWM(uint8_t index, uint16_t pwm);
  void setTriggerOuterPWM(uint8_t index, uint16_t pwm);
  void setSampling(bool sampling);
  void setLittlePacketStatus(int littlePacketStatus);

private:
  // Device members
  DeviceSettings deviceSettings;

  // Channel members
  ChannelSettings channels[2];
  ChannelSelector selectedChannel;

  // Trigger members
  TriggerSettings triggerSettings;

  // Misc
  int littlePacketStatus;
  bool sampling = false;
};

#endif // _IDSO1070_H_