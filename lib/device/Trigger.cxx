#include "Trigger.h"

uint16_t Trigger::getBottomPWM()
{
}
uint16_t Trigger::getTopPWM()
{
}
void Trigger::print()
{
}

void Trigger::setTriggerLevel(uint16_t i)
{
}

void Trigger::setChannel(TriggerChannel channel)
{
}
TriggerChannel Trigger::getChannel()
{
}

void Trigger::setSlope(TriggerSlope slope)
{
}
TriggerSlope Trigger::getSlope()
{
}

void Trigger::setMode(TriggerMode mode)
{
}
TriggerMode Trigger::getMode()
{
}

double Trigger::getXPosition()
{
}
void Trigger::setXPosition(double xPosition)
{
}

void Trigger::setInnerTriggerPWM(uint8_t index, uint16_t pwm)
{
}
uint16_t Trigger::getInnerTriggerPWM(uint8_t index)
{
}
uint16_t *Trigger::getInnerTriggerPWM()
{
}

void Trigger::setOuterTriggerPWM(uint8_t index, uint16_t pwm)
{
}
uint16_t Trigger::getOuterTriggerPWM(uint8_t index)
{
}
uint16_t *Trigger::getOuterTriggerPWM()
{
}