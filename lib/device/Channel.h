#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include <cstdio>
#include <cstdint>

#include "../util/hexdump.h"
#include "../enums.h"

class Channel
{
private:
  bool enabled = true;
  VoltageDiv verticalDiv = VDIV_1V;
  InputCoupling coupling = COUPLING_AC;
  int verticalPosition;
  uint16_t pwmArray[9][2];

  double voltage125;
  double voltageRL1;
  double voltageRL2;

public:
  void print();

  void enable();
  void disable();
  bool isEnabled();

  VoltageDiv getVerticalDiv();
  void setVerticalDiv(VoltageDiv verticalDiv);

  InputCoupling getCoupling();
  void setCoupling(InputCoupling coupling);

  int getVerticalPosition();
  void setVerticalPosition(int verticalPosition);

  uint16_t getPWM(uint8_t a, uint8_t b);
  uint16_t *getPWM(uint8_t a);
  void setPWM(uint16_t pwm, uint8_t a, uint8_t b);

  double getVoltage125();
  void setVoltage125(double voltage);

  double getVoltageRL1();
  void setVoltageRL1(double voltage);

  double getVoltageRL2();
  void setVoltageRL2(double voltage);
};

#endif // _CHANNEL_H_