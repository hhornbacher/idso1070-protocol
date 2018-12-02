#include <Protocol.h>

#include <util.h>
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>

Protocol::Protocol(const char *device) : port_(ioService_), device_(device), requestTimer_(ioService_, boost::posix_time::milliseconds(0))
{
}

void Protocol::start()
{
  if (running_)
  {
    throw Exception("Protocol is running already!");
  }

  boost::system::error_code ec;
  printf("Connecting\n");
  port_.open(device_.c_str(), ec);

  if (ec)
  {
    throw Exception(ec.message());
  }
  port_.set_option(boost::asio::serial_port_base::baud_rate(4000000));
  port_.set_option(boost::asio::serial_port_base::character_size(8));
  port_.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
  port_.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
  port_.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

  boost::thread ioServiceThread(boost::bind(&boost::asio::io_service::run, &ioService_));

  boost::thread stateMachineThread(boost::bind(&Protocol::stateMachine, this));

  running_ = true;
}

void Protocol::stop()
{
  if (running_)
  {
    boost::mutex::scoped_lock look(mutex_);
    running_ = false;
    port_.cancel();
    port_.close();
    ioService_.stop();
    ioService_.reset();
  }
}

void Protocol::stateMachine()
{
  while (running_)
  {
    switch (state_)
    {
    case Idle:
      break;
    case SendRequest:
      break;
    case ReceiveResponse:
      break;
    case Completed:
      break;
    case Cancled:
      break;
    case Sampling:
      break;
    }
  }
}

void Protocol::sendRequest(Request &request, Response &response)
{
  writeRequest(request);
  readResponse(response);
}

void Protocol::writeRequest(Request &request)
{
  requestTimer_.wait();
  boost::system::error_code ec;

  printf("%s", request.toString().c_str());

  boost::asio::write(port_, request.getBuffer(), ec);
  if (ec)
  {
    throw Exception(ec.message());
  }

  requestTimer_.expires_from_now(boost::posix_time::milliseconds(150));
}

void Protocol::readResponse(Response &response)
{
  boost::system::error_code ec;
  boost::asio::streambuf responseBuffer;

  boost::asio::read(port_, responseBuffer, boost::asio::transfer_at_least(Response::Size), ec);
  if (ec)
  {
    throw Exception(ec.message());
  }

  response.setData(responseBuffer);

  printf("%s", response.toString().c_str());
}

std::string Protocol::getFPGAFirmwareVersion()
{
  Response response;
  Request request(Request::Control, Request::ReadFPGAFirmwareVersion);

  sendRequest(request, response);
  const uint8_t *data = &response.getPayload()[6];
  char buffer[9];
  snprintf(
      buffer, 9,
      "%u%u%u%u%u%u%u%u",
      data[0], data[1],
      data[2], data[3],
      data[4], data[5],
      data[6], data[7]);
  return buffer;
}

std::string Protocol::getARMFirmwareVersion()
{
  Response response;
  Request request(Request::State, Request::ReadARMFirmwareVersion);

  sendRequest(request, response);
  return (const char *)response.getPayload();
}

uint8_t Protocol::getBatteryLevel()
{
  Response response;
  Request request(Request::State, Request::ReadBatteryLevel);

  sendRequest(request, response);

  return response.getPayload()[0];
}

void Protocol::readEEPROMPage(uint16_t address, std::vector<uint8_t> &buffer)
{
  Response response;
  Request request(Request::EEPROM, Request::ReadEEPROM, address);

  sendRequest(request, response);

  const uint8_t *data = response.getPayload();
  buffer.insert(buffer.begin(), data, &data[100]);
}

void Protocol::updateSampleRate()
{
  Response response;
  Request request(Request::FPGA, Request::SampleRate);

  sendRequest(request, response);
}

void Protocol::setFreqDiv(uint32_t freqDiv)
{
  Response response1;
  Response response2;
  uint16_t lowBytes = (uint16_t)(freqDiv & 0xffff);
  uint16_t highBytes = (uint16_t)((freqDiv >> 16) & 0xffff);
  Request request1(Request::FPGA, Request::FrequencyDivLow, lowBytes);
  Request request2(Request::FPGA, Request::FrequencyDivHigh, highBytes);

  sendRequest(request1, response1);
  sendRequest(request2, response2);
}

void Protocol::setRAMChannelSelection(bool channel1Enabled, bool channel2Enabled)
{
  Response response;
  uint8_t config = 0x01;
  if (channel1Enabled && !channel2Enabled)
  {
    config = 0x08;
  }
  else if (channel2Enabled && !channel1Enabled)
  {
    config = 0x09;
  }
  else if (channel2Enabled && channel1Enabled)
  {
    config = 0x00;
  }
  Request request(Request::FPGA, Request::RAMChannelSelection, config, 0);

  sendRequest(request, response);
}

void Protocol::setChannelSelection(bool channel1Enabled, bool channel2Enabled)
{
  Response response;
  uint8_t channelCount = channel1Enabled && channel2Enabled ? 2 : channel1Enabled || channel2Enabled ? 1 : 0;
  Request request(Request::FPGA, Request::ChannelSelection, channelCount);

  sendRequest(request, response);
}

void Protocol::setPreTriggerLength(uint16_t samplesPerFrame, double triggerXPosition)
{
  Response response;
  uint16_t config = ((uint16_t)(((double)samplesPerFrame) * triggerXPosition)) + 5;
  Request request(Request::FPGA, Request::PreTriggerLength, config);

  sendRequest(request, response);
}

void Protocol::setPostTriggerLength(uint16_t samplesPerFrame, double triggerXPosition)
{
  Response response;
  uint16_t config = ((uint16_t)(((double)samplesPerFrame) * (1 - triggerXPosition)));
  Request request(Request::FPGA, Request::PostTriggerLength, config);

  sendRequest(request, response);
}

void Protocol::setTriggerConfig(TriggerChannel triggerChanel, ScopeMode scoMode, TriggerSlope triggerSlope)
{
  Response response;
  uint8_t config =
      triggerChanel == Channel1 ? 0x01 : triggerChanel == Channel2 ? 0x00 : triggerChanel == External ? 0x02 : 0x03;

  config &= ~0x2c;
  if (scoMode == Analog)
    config |= 0x10;
  else
    config &= ~0x10;
  if (triggerSlope == Rising)
    config |= 0x80;
  else
    config &= ~0x80;

  Request request(Request::FPGA, Request::TriggerSource, config);

  sendRequest(request, response);
}

void Protocol::setTriggerMode(CaptureMode capMode, TriggerMode trigMode, ScopeMode scoMode)
{
  Response response;
  uint8_t config = 0;
  if (capMode == CaptureRoll)
    config = (1 << 0);
  else if (capMode != CaptureNormal)
    config |= (1 << 3);
  if (trigMode == Auto)
    config |= (1 << 1);
  else if (trigMode == Single)
    config |= (1 << 2);
  if (scoMode == Digital)
    config |= (1 << 4);

  Request request(Request::FPGA, Request::TriggerMode, config);

  sendRequest(request, response);
}

void Protocol::setChannel1Coupling(InputCoupling coupling)
{
  Response response;
  uint8_t param1 = 0, param2 = 0;
  if (coupling == GND)
  {
    param1 = 0xff;
    param2 = 0x01;
  }
  else if (coupling == DC)
  {
    param1 = 0xef;
  }
  else
  {
    param1 = 0x10;
  }
  Request request(Request::FPGA, Request::SetRelay, param1, param2);

  sendRequest(request, response);
}

void Protocol::setChannel2Coupling(InputCoupling coupling)
{
  Response response;
  uint8_t param1 = 0, param2 = 0;
  if (coupling == GND)
  {
    param1 = 0xff;
    param2 = 0x02;
  }
  else if (coupling == DC)
  {
    param1 = 0xfe;
  }
  else
  {
    param1 = 0x01;
  }

  Request request(Request::FPGA, Request::SetRelay, param1, param2);

  sendRequest(request, response);
}

void Protocol::setChannel1VoltageDiv(VoltageDiv voltageDiv)
{
  Response response1;
  Response response2;
  uint8_t config1 = 0;
  uint8_t config2 = 0;

  switch (voltageDiv)
  {
  case Div10mV:
  case Div100mV:
  case Div20mV:
  case Div200mV:
  case Div50mV:
  case Div500mV:
    config1 = 0x7f;
    break;
  case Div1V:
  case Div2V:
  case Div5V:
    config1 = 0x80;
    break;
  }

  switch (voltageDiv)
  {
  case Div10mV:
  case Div100mV:
  case Div20mV:
  case Div200mV:
  case Div50mV:
  case Div500mV:
    config2 = 0xfd;
    break;
  case Div1V:
  case Div2V:
  case Div5V:
    config2 = 0x02;
    break;
  }

  Request request1(Request::FPGA, Request::SetRelay, config1, 0);
  Request request2(Request::FPGA, Request::SetRelay, config2, 0);

  sendRequest(request1, response1);
  sendRequest(request1, response2);
}

void Protocol::setChannel2VoltageDiv(VoltageDiv voltageDiv)
{
  Response response1;
  Response response2;
  uint8_t config1 = 0;
  uint8_t config2 = 0;

  switch (voltageDiv)
  {
  case Div10mV:
  case Div100mV:
  case Div20mV:
  case Div200mV:
  case Div50mV:
  case Div500mV:
    config1 = 0xbf;
    break;
  case Div1V:
  case Div2V:
  case Div5V:
    config1 = 0x40;
    break;
  }

  switch (voltageDiv)
  {
  case Div10mV:
  case Div100mV:
  case Div20mV:
  case Div200mV:
  case Div50mV:
  case Div500mV:
    config2 = 0xfb;
    break;
  case Div1V:
  case Div2V:
  case Div5V:
    config2 = 0x04;
    break;
  }

  Request request1(Request::FPGA, Request::SetRelay, config1, 0);
  Request request2(Request::FPGA, Request::SetRelay, config2, 0);

  sendRequest(request1, response1);
  sendRequest(request1, response2);
}

void Protocol::setChannel1Level(int16_t verticalPosition, float level0, float level1)
{
  Response response;
  uint16_t pwm = (uint16_t)mapValue(
      verticalPosition,
      8.0f, 248.0f,
      level0, level1);

  if (pwm > MaxPWM)
  {
    throw Exception("Channel 1 PWM higher than maximum");
  }

  Request request(Request::FPGA, Request::Channel1Level, pwm);

  sendRequest(request, response);
}

void Protocol::setChannel2Level(int16_t verticalPosition, float level0, float level1)
{
  Response response;
  uint16_t pwm = (uint16_t)mapValue(
      verticalPosition,
      8.0f, 248.0f,
      level0, level1);

  if (pwm > MaxPWM)
  {
    throw Exception("Channel 2 PWM higher than maximum");
  }

  Request request(Request::FPGA, Request::Channel2Level, pwm);

  sendRequest(request, response);
}

void Protocol::setTriggerLevel(uint16_t triggerLevel, float topLevel, float bottomLevel)
{
  Response response;
  uint16_t pwm = mapValue(
      triggerLevel,
      8.0f, 248.0f,
      topLevel, bottomLevel);

  if (pwm > MaxPWM)
  {
    throw Exception("Channel 2 PWM higher than maximum");
  }

  Request request(Request::FPGA, Request::TriggerLevel, pwm);

  sendRequest(request, response);
}

void Protocol::setChannelVolts125(VoltageDiv voltageDivChannel1, VoltageDiv voltageDivChannel2)
{
  Response response;
  uint8_t config = 0;
  switch (voltageDivChannel1)
  {
  case Div10mV:
  case Div100mV:
  case Div1V:
    config &= ~0x03;
    break;
  case Div20mV:
  case Div200mV:
  case Div2V:
    config &= ~0x02;
    config |= 0x01;
    break;
  case Div50mV:
  case Div500mV:
  case Div5V:
    config &= ~0x01;
    config |= 0x02;
    break;
  }

  switch (voltageDivChannel2)
  {
  case Div10mV:
  case Div100mV:
  case Div1V:
    config &= ~0x0c;
    break;
  case Div20mV:
  case Div200mV:
  case Div2V:
    config &= ~0x08;
    config |= 0x04;
    break;
  case Div50mV:
  case Div500mV:
  case Div5V:
    config &= ~0x04;
    config |= 0x08;
    break;
  }
  config &= ~0x30;

  Request request(Request::FPGA, Request::ChannelVoltsDiv125, config, 0);

  sendRequest(request, response);
}

void Protocol::getRAMCount(int channelCount, uint16_t samplesPerFrame, double triggerXPosition, uint8_t packetsNumber)
{
  Response response;
  uint16_t config = 0;
  if (channelCount == 2)
  {
    config = samplesPerFrame;
  }
  else if (channelCount == 1)
  {
    double x = ((double)samplesPerFrame * triggerXPosition / 2) +
               ((double)samplesPerFrame * (1 - triggerXPosition));
    config = (uint16_t)x;
  }
  config &= 0x0fff;
  config += (packetsNumber - 1) << 12;

  Request request(Request::FPGA, Request::ReadRAMCount, config);

  sendRequest(request, response);
}

void Protocol::startSampling()
{
  Response response;
  Request request(Request::Control, Request::StartSampling);

  sendRequest(request, response);
}