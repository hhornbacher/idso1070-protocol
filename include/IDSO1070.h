#ifndef _IDSO1070_H_
#define _IDSO1070_H_

#include "base.h"

class IDSO1070
{
public:
  static const int MaxPWM = 4095;
  static const int MemoryDepth = 2000;
  static const int SamplesCountPerPacket = 500;
  static const int MaxSample = 248;

  struct ChannelSettings
  {
    bool enabled = true;
    VoltageDiv verticalDiv = VDIV_1V;
    InputCoupling coupling = COUPLING_AC;
    int16_t verticalPosition;
    uint16_t pwmArray[9][2];

    double voltage125;
    double voltageRL1;
    double voltageRL2;
  };
  struct TriggerSettings
  {
    bool isHold = false;
    TriggerMode mode = TRIGMODE_AUTO;
    TriggerChannel channel = TRIGCHAN_CH1;
    TriggerSlope slope = TRIGSLOPE_RISING;
    uint16_t level = 0;
    double xPosition = 0.5;
    uint16_t innerPWM[4];
    uint16_t outerPWM[2];
  };
  struct DeviceSettings
  {
    // freqDiv an timeBase are related to each other!
    TimeBase timeBase = HDIV_1mS;
    uint32_t freqDiv = 0;

    ScopeMode scopeMode = SCOMODE_ANALOG;
    CaptureMode captureMode = CAPMODE_NORMAL;

    uint8_t batteryLevel = 0;

    string armFirmwareVersion;
    string fpgaFirmwareVersion;
    string productName;
    string userName;

    uint8_t diffFixData[2][256];
    uint16_t caliLevel[100];
    uint8_t fpgaAlert[40];
  };

  IDSO1070();
  ~IDSO1070()
  {
  }

  // Copy assignment operator
  IDSO1070 &operator=(IDSO1070 obj);

  // General device settings
  void setDeviceTimeBase(TimeBase timeBase);
  TimeBase getDeviceTimeBase();

  void setDeviceCaptureMode(CaptureMode captureMode);
  CaptureMode getDeviceCaptureMode();

  void setDeviceScopeMode(ScopeMode scopeMode);
  ScopeMode getDeviceScopeMode();

  void setARMFirmwareVersion(string version);
  string getARMFirmwareVersion();

  void setFPGAFirmwareVersion(string version);
  string getFPGAFirmwareVersion();

  void setProductName(string productName);
  string getProductName();

  void setUserName(string userName);
  string getUserName();

  void setCaliLevel(uint8_t *data);
  uint16_t *getCaliLevel();

  void setFPGAAlert(uint8_t *data);
  uint8_t *getFPGAAlert();

  void setDiffFixData(size_t channel, size_t offset, uint8_t *data);

  void setBatteryLevel(uint8_t batteryLevel);
  uint8_t getBatteryLevel();

  void setFreqDiv(uint32_t freqDiv);
  uint32_t getFreqDiv();

  // Channel data access methods
  void setSelectedChannel(ChannelSelector channel);
  ChannelSelector getSelectedChannel();

  void enableChannel(ChannelSelector channel);
  void disableChannel(ChannelSelector channel);
  bool isChannelEnabled(ChannelSelector channel);

  void setChannelVerticalDiv(ChannelSelector channel, VoltageDiv verticalDiv);
  VoltageDiv getChannelVerticalDiv(ChannelSelector channel);

  void setChannelCoupling(ChannelSelector channel, InputCoupling coupling);
  InputCoupling getChannelCoupling(ChannelSelector channel);

  void setChannelVerticalPosition(ChannelSelector channel, int16_t verticalPosition);
  int16_t getChannelVerticalPosition(ChannelSelector channel);

  void setChannelPWM(ChannelSelector channel, uint16_t pwm, uint8_t a, uint8_t b);
  uint16_t getChannelPWM(ChannelSelector channel, uint8_t a, uint8_t b);
  uint16_t *getChannelPWM(ChannelSelector channel, uint8_t a);

  void setChannelVoltage125(ChannelSelector channel, double voltage);
  double getChannelVoltage125(ChannelSelector channel);

  void setChannelVoltageRL1(ChannelSelector channel, double voltage);
  double getChannelVoltageRL1(ChannelSelector channel);

  void setChannelVoltageRL2(ChannelSelector channel, double voltage);
  double getChannelVoltageRL2(ChannelSelector channel);

  // Trigger data access methods
  void setTriggerLevel(uint16_t i);
  uint16_t getTriggerLevel();

  void setTriggerChannel(TriggerChannel channel);
  TriggerChannel getTriggerChannel();

  void setTriggerSlope(TriggerSlope slope);
  TriggerSlope getTriggerSlope();

  void setTriggerMode(TriggerMode mode);
  TriggerMode getTriggerMode();

  double getTriggerXPosition();
  void setTriggerXPosition(double xPosition);

  void setTriggerInnerPWM(uint8_t index, uint16_t pwm);
  uint16_t getTriggerInnerPWM(uint8_t index);
  uint16_t *getTriggerInnerPWM();

  void setTriggerOuterPWM(uint8_t index, uint16_t pwm);
  uint16_t getTriggerOuterPWM(uint8_t index);
  uint16_t *getTriggerOuterPWM();

  uint16_t getTriggerBottomPWM();
  uint16_t getTriggerTopPWM();

  // Misc
  void setLittlePacketStatus(int littlePacketStatus);
  int getLittlePacketStatus();

  void setReceiveFreqDivStatus(uint8_t receiveFreqDivStatus);
  uint8_t getReceiveFreqDivStatus();

  bool isSampleRate200Mor250M();

  TimeBase getDeviceTimeBaseFromFreqDiv();

  size_t getSamplesNumberOfOneFrame();
  uint8_t getEnabledChannelsCount();
  uint8_t getPacketsNumber();

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
  uint8_t receiveFreqDivStatus;
};

#endif // _IDSO1070_H_