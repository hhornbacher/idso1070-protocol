#include "Channel.h"

Channel::Channel() : sampleBuffer(SampleBufferSize)
{
}

void Channel::print()
{
    printf("[Channel]\n");
    printf("enabled = %d\n", enabled);
    printf("verticalDiv = %d\n", verticalDiv);
    printf("coupling = %d\n", coupling);
    printf("verticalPosition = %d\n", coupling);
    printf("voltage125 = %lf\n", voltage125);
    printf("voltageRL1 = %lf\n", voltageRL1);
    printf("voltageRL2 = %lf\n", voltageRL2);
    for (int i = 0; i < 9; i++)
    {
        printf("pwmArray[%d][0] = %d\n", i, pwmArray[i][0]);
        printf("pwmArray[%d][1] = %d\n", i, pwmArray[i][1]);
    }
    printf("\n\n");
}

void Channel::enable()
{
    enabled = true;
}
void Channel::disable()
{
    enabled = false;
}
bool Channel::isEnabled()
{
    return enabled;
}

VoltageDiv Channel::getVerticalDiv()
{
    return verticalDiv;
}
void Channel::setVerticalDiv(VoltageDiv verticalDiv)
{
    this->verticalDiv = verticalDiv;
}

InputCoupling Channel::getCoupling()
{
    return coupling;
}
void Channel::setCoupling(InputCoupling coupling)
{
    this->coupling = coupling;
}

int16_t Channel::getVerticalPosition()
{
    return verticalPosition;
}
void Channel::setVerticalPosition(int16_t verticalPosition)
{
    if (verticalPosition < 8)
        verticalPosition = 8;
    else if (verticalPosition > MaxSample)
        verticalPosition = MaxSample;
    this->verticalPosition = verticalPosition;
}

uint16_t Channel::getPWM(uint8_t a, uint8_t b)
{
    if (a < 9 && b < 2)
        return pwmArray[a][b];
    return -1;
}
uint16_t *Channel::getPWM(uint8_t a)
{
    if (a < 9)
        return pwmArray[a];
    return NULL;
}

void Channel::setPWM(uint16_t pwm, uint8_t a, uint8_t b)
{
    pwmArray[a][b] = pwm;
}

double Channel::getVoltage125()
{
    return voltage125;
}
void Channel::setVoltage125(double voltage)
{
    this->voltage125 = voltage;
}

double Channel::getVoltageRL1()
{
    return voltageRL1;
}
void Channel::setVoltageRL1(double voltage)
{
    this->voltageRL1 = voltage;
}

double Channel::getVoltageRL2()
{
    return voltageRL2;
}
void Channel::setVoltageRL2(double voltage)
{
    this->voltageRL2 = voltage;
}

Channel::SampleBuffer &Channel::getSampleBuffer()
{
    return sampleBuffer;
}