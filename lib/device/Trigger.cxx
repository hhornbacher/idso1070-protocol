#include "Trigger.h"
#include "IDSO1070A.h"

void Trigger::print()
{
    printf("[Trigger]\n");
    printf("isHold = %d\n", isHold);
    printf("mode = %d\n", mode);
    printf("channel = %d\n", channel);
    printf("slope = %d\n", slope);
    printf("level = %d\n", level);
    printf("xPosition = %lf\n", xPosition);
    printf("innerTriggerPWM = \n");
    hexdump((uint8_t *)innerTriggerPWM, sizeof(innerTriggerPWM));
    printf("outerTriggerPWM = \n");
    hexdump((uint8_t *)outerTriggerPWM, sizeof(outerTriggerPWM));
    printf("\n\n");
}

uint16_t Trigger::getBottomPWM()
{
    if (channel == TRIGCHAN_CH1)
        return innerTriggerPWM[0];
    else if (channel == TRIGCHAN_CH2)
        return innerTriggerPWM[2];
    return -1;
}

uint16_t Trigger::getTopPWM()
{
    if (channel == TRIGCHAN_CH1)
        return innerTriggerPWM[1];
    else if (channel == TRIGCHAN_CH2)
        return innerTriggerPWM[3];
    return -1;
}

void Trigger::setTriggerLevel(uint16_t i)
{
    if (i < 8)
    {
        i = 8;
    }
    else if (i > IDSO1070A_MAX_SAMPLE)
    {
        i = IDSO1070A_MAX_SAMPLE;
    }
    level = i;
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

void Trigger::setInnerTriggerPWM(uint8_t index, uint16_t pwm)
{
    this->innerTriggerPWM[index] = pwm;
}
uint16_t Trigger::getInnerTriggerPWM(uint8_t index)
{
    return innerTriggerPWM[index];
}
uint16_t *Trigger::getInnerTriggerPWM()
{
    return innerTriggerPWM;
}

void Trigger::setOuterTriggerPWM(uint8_t index, uint16_t pwm)
{
    this->outerTriggerPWM[index] = pwm;
}
uint16_t Trigger::getOuterTriggerPWM(uint8_t index)
{
    return outerTriggerPWM[index];
}
uint16_t *Trigger::getOuterTriggerPWM()
{
    return outerTriggerPWM;
}