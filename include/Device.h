#pragma once

class Device
{
public:
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
};