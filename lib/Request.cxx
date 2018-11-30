#include <Request.h>

#include <sstream>

using namespace std;

Request::Request(Type type, Command command, uint16_t params)
{
  data_.push_back((uint8_t)type);
  data_.push_back((uint8_t)command);
  data_.push_back((uint8_t)(params & 0xff));
  data_.push_back((uint8_t)((params >> 8) & 0xff));
}

Request::Request(Type type, Command command, uint8_t param1, uint8_t param2)
{
  data_.push_back((uint8_t)type);
  data_.push_back((uint8_t)command);
  data_.push_back(param1);
  data_.push_back(param2);
}

string Request::toString() const
{
  stringstream ss;
  ss << "Request:\n";
  ss << "Type: ";
  switch ((Type)data_[0])
  {
  case Control:
    ss << "Control\n";
    break;
  case FPGA:
    ss << "FPGA\n";
    break;
  case EEPROM:
    ss << "EEPROM\n";
    break;
  case State:
    ss << "State\n";
    break;
  default:
    ss << "Unkown\n";
  }

  ss << "Command: ";
  switch ((Command)data_[1])
  {
  case ForceTrigger:
    ss << "ForceTrigger\n";
    break;
  case TriggerMode: // also ReadFPGAFirmwareVersion
    if ((Type)data_[0] == FPGA)
    {
      ss << "TriggerMode\n";
    }
    else
    {
      ss << "ReadFPGAFirmwareVersion\n";
    }
    break;
  case SetRelay: // also ReadBatteryLevel
    if ((Type)data_[0] == State)
    {
      ss << "ReadBatteryLevel\n";
    }
    else
    {
      ss << "SetRelay\n";
    }
    break;
  case ChannelSelection: // also ReadARMFirmwareVersion and StartSampling
    if ((Type)data_[0] == FPGA)
    {
      ss << "ChannelSelection\n";
    }
    else if ((Type)data_[0] == Control)
    {
      ss << "StartSampling\n";
    }
    else
    {
      ss << "ReadARMFirmwareVersion\n";
    }
    break;
  case ReadEEPROM:
    ss << "ReadEEPROM\n";
    break;
  case TriggerSource:
    ss << "TriggerSource\n";
    break;
  case ChannelVoltsDiv125:
    ss << "ChannelVoltsDiv125\n";
    break;
  case PreTriggerLength:
    ss << "PreTriggerLength\n";
    break;
  case PostTriggerLength:
    ss << "PostTriggerLength\n";
    break;
  case RAMStartPosition:
    ss << "RAMStartPosition\n";
    break;
  case ReservedDataOutput:
    ss << "ReservedDataOutput\n";
    break;
  case Channel1PWM:
    ss << "Channel1PWM\n";
    break;
  case Channel2PWM:
    ss << "Channel2PWM\n";
    break;
  case TriggerPWM:
    ss << "TriggerPWM\n";
    break;
  case LogicAnalyzer1:
    ss << "LogicAnalyzer1\n";
    break;
  case LogicAnalyzer2:
    ss << "LogicAnalyzer2\n";
    break;
  case LogicAnalyzer3:
    ss << "LogicAnalyzer3\n";
    break;
  case SampleRate:
    ss << "SampleRate\n";
    break;
  case FrequencyDivLow:
    ss << "FrequencyDivLow\n";
    break;
  case FrequencyDivHigh:
    ss << "FrequencyDivHigh\n";
    break;
  case SerialBaudRate:
    ss << "SerialBaudRate\n";
    break;
  case RAMChannelSelection:
    ss << "RAMChannelSelection\n";
    break;
  case ReadRAMCount:
    ss << "ReadRAMCount\n";
    break;
  default:
    ss << "Unkown\n";
  }

  ss << Packet::toString();
  return ss.str();
}