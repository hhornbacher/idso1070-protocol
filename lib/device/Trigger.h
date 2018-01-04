#ifndef _TRIGGER_H_
#define _TRIGGER_H_

#include <cstdio>
#include <cstdint>

#include "../util/hexdump.h"
#include "../enums.h"

class Trigger
{
private:
  bool isHold = false;
  TriggerMode mode = TRIGMODE_AUTO;
  TriggerChannel channel = TRIGCHAN_CH1;
  TriggerSlope slope = TRIGSLOPE_RISING;
  uint16_t level = 0;
  double xPosition = 0.5;
  uint16_t innerTriggerPWM[4];
  uint16_t outerTriggerPWM[2];

public:
  uint16_t getBottomPWM();
  uint16_t getTopPWM();
  void print();

  void setTriggerLevel(uint16_t i);

  void setChannel(TriggerChannel channel);
  TriggerChannel getChannel();

  void setSlope(TriggerSlope slope);
  TriggerSlope getSlope();

  void setMode(TriggerMode mode);
  TriggerMode getMode();

  double getXPosition();
  void setXPosition(double xPosition);

  void setInnerTriggerPWM(uint8_t index, uint16_t pwm);
  uint16_t getInnerTriggerPWM(uint8_t index);
  uint16_t *getInnerTriggerPWM();

  void setOuterTriggerPWM(uint8_t index, uint16_t pwm);
  uint16_t getOuterTriggerPWM(uint8_t index);
  uint16_t *getOuterTriggerPWM();
};

#endif // _TRIGGER_H_