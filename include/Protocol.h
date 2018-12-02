#pragma once

#include "Request.h"
#include "Response.h"
#include "SampleStream.h"

#include <stdexcept>
#include <vector>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

class Protocol
{
public:
  static constexpr int MaxPWM = 4095;

  enum TriggerChannel
  {
    Channel1,
    Channel2,
    External,
    Ext10,
    Alt
  };

  enum TriggerMode
  {
    Auto,
    Normal,
    Single
  };

  enum TriggerSlope
  {
    Rising,
    Falling
  };

  enum ScopeMode
  {
    Analog,
    Digital
  };

  enum CaptureMode
  {
    CaptureRoll,
    CaptureScan,
    CaptureNormal
  };

  enum InputCoupling
  {
    AC,
    DC,
    GND
  };

  enum VoltageDiv
  {
    Div10mV,
    Div20mV,
    Div50mV,
    Div100mV,
    Div200mV,
    Div500mV,
    Div1V,
    Div2V,
    Div5V
  };

  Protocol(const char *device);

  void start();
  void stop();

  void sendRequest(Request &request, Response &response);
  void writeRequest(Request &request);
  void readResponse(Response &response);

  std::string getFPGAFirmwareVersion();
  std::string getARMFirmwareVersion();
  uint8_t getBatteryLevel();
  void readEEPROMPage(uint16_t address, std::vector<uint8_t> &buffer);
  void updateSampleRate();
  void setFreqDiv(uint32_t freqDiv);
  void setRAMChannelSelection(bool channel1Enabled, bool channel2Enabled);
  void setChannelSelection(bool channel1Enabled, bool channel2Enabled);
  void setPreTriggerLength(uint16_t samplesPerFrame, double triggerXPosition);
  void setPostTriggerLength(uint16_t samplesPerFrame, double triggerXPosition);
  void setTriggerConfig(TriggerChannel triggerChanel, ScopeMode scoMode, TriggerSlope triggerSlope);
  void setTriggerMode(CaptureMode capMode, TriggerMode trigMode, ScopeMode scoMode);
  void setChannel1Coupling(InputCoupling coupling);
  void setChannel2Coupling(InputCoupling coupling);
  void setChannel1VoltageDiv(VoltageDiv voltageDiv);
  void setChannel2VoltageDiv(VoltageDiv voltageDiv);
  void setChannel1Level(int16_t verticalPosition, float level0, float level1);
  void setChannel2Level(int16_t verticalPosition, float level0, float level1);
  void setTriggerLevel(uint16_t triggerLevel, float topLevel, float bottomLevel);
  void setChannelVolts125(VoltageDiv voltageDivChannel1, VoltageDiv voltageDivChannel2);
  void getRAMCount(int channelCount, uint16_t samplesPerFrame, double triggerXPosition, uint8_t packetsNumber);

  void startSampling();

protected:
  void samplingThread();

  boost::mutex mutex_;
  boost::asio::io_service ioService_;
  boost::asio::serial_port port_;

  std::string device_;

  boost::asio::deadline_timer requestTimer_;

public:
  class Exception : public std::runtime_error
  {
  public:
    Exception(std::string reason)
        : runtime_error("protocol error"), reason_(reason)
    {
    }

    virtual const char *what() const throw()
    {
      return reason_.c_str();
    }

  private:
    std::string reason_;
  };
};