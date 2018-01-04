#include "Channel.h"

void Channel::print()
{
}

void Channel::enable()
{
}
void Channel::disable()
{
}
bool Channel::isEnabled()
{
}

VoltageDiv Channel::getVerticalDiv()
{
}
void Channel::setVerticalDiv(VoltageDiv verticalDiv)
{
}

InputCoupling Channel::getCoupling()
{
}
void Channel::setCoupling(InputCoupling coupling)
{
}

int Channel::getVerticalPosition()
{
}
void Channel::setVerticalPosition(int verticalPosition)
{
}

uint16_t Channel::getPWM(uint8_t a, uint8_t b)
{
}
uint16_t *Channel::getPWM(uint8_t a)
{
}

void Channel::setPWM(uint16_t pwm, uint8_t a, uint8_t b)
{
}

double Channel::getVoltage125()
{
}
void Channel::setVoltage125(double voltage)
{
}

double Channel::getVoltageRL1()
{
}
void Channel::setVoltageRL1(double voltage)
{
}

double Channel::getVoltageRL2()
{
}
void Channel::setVoltageRL2(double voltage)
{
}