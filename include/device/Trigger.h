#ifndef _TRIGGER_H_
#define _TRIGGER_H_

#include "base.h"
#include "enums.h"
#include "Channel.h"

class Trigger
{
public:
  uint16_t getBottomPWM();
  uint16_t getTopPWM();

  void setLevel(uint16_t i);
  uint16_t getLevel();

  void setChannel(TriggerChannel channel);
  TriggerChannel getChannel();

  void setSlope(TriggerSlope slope);
  TriggerSlope getSlope();

  void setMode(TriggerMode mode);
  TriggerMode getMode();

  double getXPosition();
  void setXPosition(double xPosition);

  void setInnerPWM(uint8_t index, uint16_t pwm);
  uint16_t getInnerPWM(uint8_t index);
  uint16_t *getInnerPWM();

  void setOuterPWM(uint8_t index, uint16_t pwm);
  uint16_t getOuterPWM(uint8_t index);
  uint16_t *getOuterPWM();

private:
  bool isHold = false;
  TriggerMode mode = TRIGMODE_AUTO;
  TriggerChannel channel = TRIGCHAN_CH1;
  TriggerSlope slope = TRIGSLOPE_RISING;
  uint16_t level = 0;
  double xPosition = 0.5;
  uint16_t innerPWM[4];
  uint16_t outerPWM[2];
};

#endif // _TRIGGER_H_