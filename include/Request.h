#pragma once

#include "Packet.h"

class Request : public Packet
{
public:
  enum Type
  {
    Control = 0xaa,
    EEPROM = 0xee,
    FPGA = 0x55,
    State = 0x57
  };

  enum Command
  {
    ReadBatteryLevel = 0x03,
    ReadARMFirmwareVersion = 0x04,
    ReadFPGAFirmwareVersion = 0x02,
    StartSampling = 0x04,
    ReadEEPROM = 0xaa,
    ForceTrigger = 0x01,
    TriggerMode = 0x02,
    SetRelay = 0x03,
    ChannelSelection = 0x04,
    TriggerSource = 0x05,
    ChannelVoltsDiv125 = 0x06,
    PreTriggerLength = 0x07,
    PostTriggerLength = 0x08,
    RAMStartPosition = 0x09,
    ReservedDataOutput = 0x0a,
    Channel1PWM = 0x0b,
    Channel2PWM = 0x0c,
    TriggerPWM = 0x0d,
    LogicAnalyzer1 = 0x0e,
    LogicAnalyzer2 = 0x0f,
    LogicAnalyzer3 = 0x10,
    SampleRate = 0x11,
    FrequencyDivLow = 0x12,
    FrequencyDivHigh = 0x13,
    SerialBaudRate = 0x14,
    RAMChannelSelection = 0x15,
    ReadRAMCount = 0x16
  };

  Request(Type type, Command command, uint16_t params = 0);
  Request(Type type, Command command, uint8_t param1, uint8_t param2);

  std::string toString();
};