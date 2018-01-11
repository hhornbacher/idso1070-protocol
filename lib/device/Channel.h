#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include <cstdio>
#include <cstdint>

#include <boost/circular_buffer.hpp>

#include "../util/hexdump.h"
#include "../enums.h"

class Channel
{
public:
  static const int MaxSample = 248;
  static const int SampleBufferSize = 6000;

  typedef boost::circular_buffer<int16_t> SampleBuffer;

private:
  bool enabled = true;
  VoltageDiv verticalDiv = VDIV_1V;
  InputCoupling coupling = COUPLING_AC;
  int16_t verticalPosition;
  uint16_t pwmArray[9][2];

  double voltage125;
  double voltageRL1;
  double voltageRL2;

  SampleBuffer sampleBuffer;

public:
  Channel();

  void print();

  void enable();
  void disable();
  bool isEnabled();

  VoltageDiv getVerticalDiv();
  void setVerticalDiv(VoltageDiv verticalDiv);

  InputCoupling getCoupling();
  void setCoupling(InputCoupling coupling);

  int16_t getVerticalPosition();
  void setVerticalPosition(int16_t verticalPosition);

  uint16_t getPWM(uint8_t a, uint8_t b);
  uint16_t *getPWM(uint8_t a);
  void setPWM(uint16_t pwm, uint8_t a, uint8_t b);

  double getVoltage125();
  void setVoltage125(double voltage);

  double getVoltageRL1();
  void setVoltageRL1(double voltage);

  double getVoltageRL2();
  void setVoltageRL2(double voltage);

  SampleBuffer &getSampleBuffer();
};

#endif // _CHANNEL_H_