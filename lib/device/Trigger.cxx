#include "device/Trigger.h"
#include "device/IDSO1070.h"

void Trigger::print()
{
    printf("[Trigger]\n");
    printf("isHold = %d\n", isHold);
    printf("mode = %d\n", mode);
    printf("channel = %d\n", channel);
    printf("slope = %d\n", slope);
    printf("level = %d\n", level);
    printf("xPosition = %lf\n", xPosition);
    for (int i = 0; i < 4; i++)
    {
        printf("innerPWM[%d] = %d\n", i, innerPWM[i]);
    }
    for (int j = 0; j < 2; j++)
    {
        printf("outerPWM[%d] = %d\n", j, outerPWM[j]);
    }
    printf("\n\n");
}

uint16_t Trigger::getBottomPWM()
{
    if (channel == TRIGCHAN_CH1)
        return innerPWM[0];
    else if (channel == TRIGCHAN_CH2)
        return innerPWM[2];
    return -1;
}

uint16_t Trigger::getTopPWM()
{
    if (channel == TRIGCHAN_CH1)
        return innerPWM[1];
    else if (channel == TRIGCHAN_CH2)
        return innerPWM[3];
    return -1;
}

void Trigger::setLevel(uint16_t i)
{
    if (i < 8)
    {
        i = 8;
    }
    else if (i > Channel::MaxSample)
    {
        i = Channel::MaxSample;
    }
    level = i;
}
uint16_t Trigger::getLevel()
{
    return level;
}

void Trigger::setChannel(TriggerChannel channel)
{
    this->channel = channel;
}
TriggerChannel Trigger::getChannel()
{
    return channel;
}

void Trigger::setSlope(TriggerSlope slope)
{
    this->slope = slope;
}
TriggerSlope Trigger::getSlope()
{
    return slope;
}

void Trigger::setMode(TriggerMode mode)
{
    this->mode = mode;
}
TriggerMode Trigger::getMode()
{
    return mode;
}

double Trigger::getXPosition()
{
    return xPosition;
}
void Trigger::setXPosition(double xPosition)
{
    this->xPosition = xPosition;
}

void Trigger::setInnerPWM(uint8_t index, uint16_t pwm)
{
    this->innerPWM[index] = pwm;
}
uint16_t Trigger::getInnerPWM(uint8_t index)
{
    return innerPWM[index];
}
uint16_t *Trigger::getInnerPWM()
{
    return innerPWM;
}

void Trigger::setOuterPWM(uint8_t index, uint16_t pwm)
{
    this->outerPWM[index] = pwm;
}
uint16_t Trigger::getOuterPWM(uint8_t index)
{
    return outerPWM[index];
}
uint16_t *Trigger::getOuterPWM()
{
    return outerPWM;
}